#include "model.h"
#include <time.h> 

int main(int argc, char* argv[]){

    double time_spent = 0.0;
    clock_t begin = clock();

    FILE *fp_in, *fp_out;
	unsigned long depth=0;
	int16_t ch;
	int16_t num_escapes;
	int searching=0;
	unsigned long idx_next_context=0;

	//index 0 reserved to ROOT
	unsigned long idx_nodes=1;
	long down_node;
	long next_context;
	long high;
	long low;
	long range;

    if(argc > 2 ){
        printf("Encoding file\n");
    }
    else {
        printf("./encode_ppm Inputfile OutputFile\n");
        return 0;
    }

	if((fp_in = fopen(argv[1],"r"))==NULL){
		printf("ERROR: No such file\n");
		return 0;
	}

    fp_out = fopen(argv[2],"wb");

	static Node *trie;
	trie = (Node*) malloc(NUM_NODES*sizeof(Node));
	//Total Count a 1 por causa do <ESC>
	set_node(&trie[0],0,0,0,0,0,1,0);

	while((ch = fgetc(fp_in)) != EOF) { 
	   num_escapes=0;
	   range = (long)(high - low) + 1;   
	   //inicia a busca do contexto
       while(!add_symbol(trie,&down_node, &next_context, ch, &depth,&idx_nodes,&searching,&idx_next_context,&num_escapes,&high,&low,range)){
		   printf("%lu %lu\n",high,low);
	   }
	   //printf("send %d <ESC>\n",num_escapes);
	} 
	
	clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("The elapsed time is %f seconds\n\n", time_spent);

	// search_left_symbol(trie,trie[trie[24].down_node].last_node_added, trie[24].symbol);
	// search_left_symbol(trie,trie[trie[10].down_node].last_node_added, trie[10].symbol);
	// search_left_symbol(trie,trie[trie[13].down_node].last_node_added, trie[13].symbol);
    

}


void set_node(Node *node, int16_t _symbol, unsigned long _cum_count, long _down, long _vine,unsigned long _first_node_added,unsigned long total_count,u_int8_t _num_descendant) {
	node->symbol = _symbol;
	node->cum_count = _cum_count;
	node->down_node = _down;
	node->next_context = _vine;
	node->first_node_added = _first_node_added;
	node->total_count = total_count;
	node->num_descendant = _num_descendant;
	node->r=0;
	node->last_node_added=0;
}

int add_symbol(Node *trie,long *down_node, long* next_context, int16_t symbol,unsigned long *depth,unsigned long *idx_nodes,int *searching,unsigned long *idx_next_context,int16_t *num_escapes,long *high, long* low,long range){
	
	long found_symb;
	long id_left;
	if ((*searching) == 0) {
		*searching=1;
		if((*depth)<MAX_ORDER+1){
			(*depth)++;
			(*next_context) = *idx_next_context;
		}
		else{ (*next_context) = trie[*idx_next_context].next_context; }
	}
	else if((*searching) == 2){ (*next_context) = trie[*next_context].next_context; }

	found_symb = search_symbol(trie,*next_context, symbol);


	//  nao econtrou simbolo 
	//  adiciona simbolo ao contexto
	//	parte para o proximo contexto
	if(found_symb == -1){
		if(trie[(*next_context)].first_node_added==0)
			trie[(*next_context)].first_node_added=*idx_nodes;

		//send a ESC signal
		(*num_escapes)++;
		
		// calcular high e low para enviar um escape
		// O Cum_Count do <ESC> vai ser sempre igual ao total count do contexto (PPMA)
		*high = *low + (range * trie[(*next_context)].total_count) / trie[(*next_context)].total_count - 1; /* region to that   */
  		*low  = *low + (range * trie[trie[(*next_context)].first_node_added].cum_count) / trie[(*next_context)].total_count;          /* allotted to this */
		
		trie[(*next_context)].total_count+=1;
		trie[(*next_context)].num_descendant++;

		//falta definir o ponteiro para o proximo contexto
		//Total Count a 1 por causa do <ESC>
		set_node(&trie[*idx_nodes],symbol,1,*next_context,0,0,1,0);

		
		
		if(trie[(*next_context)].last_node_added!=0){
			update_CumCount(trie,trie[(*next_context)].last_node_added);
			trie[*idx_nodes].r=trie[(*next_context)].last_node_added;
		}
		
		trie[(*next_context)].last_node_added = *idx_nodes;

		if ((*searching) == 1) {
			(*idx_next_context) = (*idx_nodes);
			(*searching) = 2;
		}
		else{
			trie[*idx_nodes-1].next_context = *idx_nodes;
		}

		(*idx_nodes)++;

		// recorrer ao modelo de ordem -1
		if((*next_context)==0){
			trie[*idx_nodes-1].next_context=0;
			if(depth==0){
				(*next_context) = *idx_nodes;
				(*idx_next_context) = *idx_nodes;
			}
			(*searching)=0;
			return 1;
		}
		return 0;
	}
	else{
		
		if(trie[*idx_nodes-1].next_context==0){ trie[*idx_nodes-1].next_context=found_symb;}
		else{ *idx_next_context = found_symb; }

		
		if(trie[found_symb].r==0){
			*high = *low + (range * trie[trie[found_symb].down_node].total_count) / trie[trie[found_symb].down_node].total_count - 1;
		}
		else{
			*high = *low + (range * trie[trie[found_symb].r].cum_count) / trie[trie[found_symb].down_node].total_count - 1;
		}
		 /* region to that   */
		if(found_symb==trie[trie[found_symb].down_node].last_node_added){
  			*low  = *low + (range * 0) / trie[trie[found_symb].down_node].total_count; /* allotted to this */
		}
		else{
			*low  = *low + (range * trie[found_symb].cum_count) / trie[trie[found_symb].down_node].total_count;  /* allotted to this */
		}

		update_CumCount_context(trie,found_symb);

		(*searching)=0;
		return 1;
	}

}


long search_symbol(Node *trie,long next_context, u_int8_t symbol){
	long found = -1;

	for (unsigned long i = trie[next_context].last_node_added; i !=0; i = trie[i].r){
		/* code */
		if(trie[i].symbol == symbol)
			return i;
	}
	return found;
}

long search_left_symbol(Node *trie,long index, u_int8_t symbol){
	long found = -1;
	long i_old = 0 ;
	for (unsigned long i = index; i !=0; i = trie[i].r){
		/* code */
		if(trie[i].symbol == symbol)
			return i_old;
		i_old = i;
	}
	return i_old;
}


void update_CumCount(Node *trie,long index){
	long found = -1;
	for (long i = index; i !=0; i = trie[i].r){ 
		(trie[i].cum_count)++; 
	}
}

void update_CumCount_context(Node *trie,long index){
	long found = -1;
	for (long i = index; i !=0; i = trie[i].next_context){ 
		trie[trie[i].down_node].total_count+=1;
		update_CumCount(trie,i);
	}
}

void print_right(Node *trie,long index){
	long found = -1;

	for (unsigned long i = trie[index].last_node_added; i !=0; i = trie[i].r){ printf("%c",trie[i].symbol); }
	printf("\n=========================\n");
}