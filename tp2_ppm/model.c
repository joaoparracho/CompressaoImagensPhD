#include "model.h"
#include <time.h> 

#define INIT_SMB NUM_CHARS+1

int main(int argc, char* argv[]){

    double time_spent = 0.0;
    clock_t begin = clock();

    FILE *fp_in, *fp_out;
	unsigned long depth=0;
	int init_escapes=0;
	int16_t ch;
	int searching=0;
	unsigned long idx_next_context=0;

	//index 0 reserved to ROOT
	unsigned long idx_nodes=1;
	long down_node;
	long next_context;

	// set_node(&down_node,INIT_SMB,0,NULL,NULL,0,0,0);
	// set_node(&next_context,INIT_SMB,0,NULL,NULL,0,0,0);


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
	//Node trie[NUM_NODES];
	//Node *trie = (Node*) calloc(200000,sizeof(Node));
	static Node *trie;
	trie = (Node*) malloc(NUM_NODES*sizeof(Node));
	set_node(&trie[0],0,0,0,0,0,0,0);

	for (size_t i = 0; i < MAX_ORDER; i++){
		init_escapes++;
	}
	long aaa=0;
	int a=0;
	while((ch = fgetc(fp_in)) != EOF) { 
	   //inicia a busca do contexto
	   a=0;
	   aaa++;
       while(!add_symbol(trie,&down_node, &next_context, ch, &depth,&idx_nodes,&searching,&idx_next_context,&a));
	   // printf("-%d %c\n",ch,ch);
	} 
	
	clock_t end = clock();
 
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
	// print_right(trie,0);
	// print_right(trie,1);
	// print_right(trie,3);
	// print_right(trie,6);
	// print_right(trie,13);

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("The elapsed time is %f seconds\n\n", time_spent);
    

}


void set_node(Node *node, int16_t _symbol, unsigned long _cum_count, long _down, long _vine,unsigned long _num_escapes,unsigned long total_count,u_int8_t _num_descendant) {
	node->symbol = _symbol;
	node->cum_count = _cum_count;
	node->down_node = _down;
	node->next_context = _vine;
	node->num_escapes = _num_escapes;
	node->total_count = total_count;
	node->num_descendant = _num_descendant;
	//node->right = (unsigned long*) malloc(NUM_CHARS*sizeof(unsigned long));
	node->r=0;
	node->last_node_added=0;
}

int add_symbol(Node *trie,long *down_node, long* next_context, int16_t symbol,unsigned long *depth,unsigned long *idx_nodes,int *searching,unsigned long *idx_next_context,int *a){
	
	long found_symb;
	if ((*searching) == 0) {
		*searching=1;
		if((*depth)<MAX_ORDER+1){
			(*depth)++;
			(*next_context) = *idx_next_context;
		}
		else{
			(*next_context) = trie[*idx_next_context].next_context;
		}
	}
	else if((*searching) == 2){
		(*next_context) = trie[*next_context].next_context ;
		
	}

	if((*idx_nodes)==NUM_NODES){
			printf("FUCK YOU");
	}
	
	// if(*a>=MAX_ORDER){
	// 	printf("STOp");
	// }
	// (*a)++;

	found_symb = search_symbol(trie,*next_context, symbol);


	//  nao econtrou simbolo 
	//  adiciona simbolo ao contexto
	//	parte para o proximo contexto
	if(found_symb == -1){
		//printf("%d,",*idx_nodes);
		// if(symbol=='D'){
		// 	printf("A");
		// }
		trie[(*next_context)].total_count+=1;
		trie[(*next_context)].num_descendant++;
		//trie[(*next_context)].right[trie[(*next_context)].num_descendant++]=*idx_nodes;
		
		
		// if(*next_context==0){
		// 	printf("%c\n",symbol);
		// }

		//falta definir o ponteiro para o proximo contexto
		set_node(&trie[*idx_nodes],symbol,trie[(*next_context)].total_count,*next_context,0,0,0,0);
		
		if(trie[(*next_context)].last_node_added!=0){
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

		if((*next_context)==0){
			trie[*idx_nodes-1].next_context=0;
			if(depth==0){
				(*next_context) = *idx_nodes;
				(*idx_next_context) = *idx_nodes;
			}
			(*searching)=0;
			*a=0;
			return 1;
		}
		return 0;
	}
	else{
		// if(symbol=='D'){
		// 	printf("BUG_DETECTED");
		// }
		//printf("%d ()",trie[(*next_context)].right[found_symb]);
		trie[found_symb].cum_count++;
		if(trie[*idx_nodes-1].next_context==0){
			trie[*idx_nodes-1].next_context=found_symb;
		}
		else{
			*idx_next_context = found_symb;
		}
		
	
		*a=0;
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
			//printf("%lu",i);
	}

	// for (u_int8_t i = 0; i < trie[next_context].num_descendant; i++){
	// 	/* code */
	// 	if(trie[trie[next_context].right[i]].symbol == symbol)
	// 		return i;
	// }
	return found;
}

void print_right(Node *trie,long index){
	long found = -1;

	for (unsigned long i = trie[index].last_node_added; i !=0; i = trie[i].r){
		printf("%c",trie[i].symbol);
	}
	printf("\n=========================\n");


}