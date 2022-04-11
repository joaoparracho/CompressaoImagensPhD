#include "utils.h"


int main(int argc, char* argv[]){

    FILE *fp_in, *fp_out;

    Node *nd_Huff_dec;
    Node *nd_Huff = (Node*) calloc(ASCII_SIZE,sizeof(Node));

    unsigned int *nd_idx = (unsigned int*) calloc(ASCII_SIZE,sizeof(unsigned int));
    char num_unique=0;
    unsigned int num_total=0;
    unsigned int num_leafnintern=0;
    unsigned int id1=0;
    unsigned int id2=0;
    unsigned int compressedBits=0;

    char numBits=0;
    char ch_aux=0;
    char ch;
    
    float prob=0;
    
    if(argc == 4){
        printf("Reading File\n");
    }
    else {
        printf("ERROR: Insert path to file\n");
        return 0;
    }

	if((fp_in = fopen(argv[1],"r"))==NULL){
		printf("ERROR: No such file\n");
		return 0;
	}

    fp_out = fopen(argv[2],"wb");

    while((ch = fgetc(fp_in)) != EOF) { 
        nd_Huff[(int)ch].num_occur++;
        num_total++;
    } 

    for (int i = 0; i < ASCII_SIZE; i++){
        nd_Huff[i].ch=(char)i;
        nd_idx[i]=i;
        if(nd_Huff[i].num_occur!=0) num_unique++;
    }
    
    orderNodes(nd_Huff,nd_idx,ASCII_SIZE);
   
    for (int i = 0; i < num_unique; i++){
        printf(" %c - %d - %0.5f \n",nd_Huff[nd_idx[i]].ch,nd_Huff[nd_idx[i]].num_occur,(float)nd_Huff[nd_idx[i]].num_occur/num_total);
        prob+=(float)nd_Huff[nd_idx[i]].num_occur/num_total;
    }
    num_leafnintern=(num_unique<<1)-1;

    if(num_leafnintern>ASCII_SIZE){
        nd_Huff = (Node *)realloc(nd_Huff,(num_leafnintern-ASCII_SIZE)*sizeof(Node));
        nd_idx = (unsigned int *)realloc(nd_idx,(num_leafnintern-ASCII_SIZE)*sizeof(unsigned int));
        for (size_t i = ASCII_SIZE; i <num_leafnintern ; i++){
            nd_idx[i]=i;
        }
    }

    buildTree(nd_Huff,nd_idx, num_unique);

    //print_nodes(nd_Huff,nd_idx,num_leafnintern);

    buildCodes(nd_Huff,nd_idx,num_leafnintern);

    print_codes(nd_Huff,nd_idx,num_leafnintern);

    for (int i = 0; i < num_leafnintern; i++){
        printf("\n %c - %d - %0.5f",nd_Huff[nd_idx[i]].ch,nd_Huff[nd_idx[i]].num_occur,(float)nd_Huff[nd_idx[i]].num_occur/num_total);
        prob+=(float)nd_Huff[nd_idx[i]].num_occur/num_total;
    }

    
    fseek(fp_in, 0, SEEK_SET); //rewind the text file to the beggining
    putw(num_unique,fp_out);
    putw(compressedBits,fp_out);
    
    for(int i = 0; i < num_leafnintern; i++){
        
        if(nd_Huff[nd_idx[i]].type==LEAF){
            fputc(nd_Huff[nd_idx[i]].ch,fp_out);
            putw(nd_Huff[nd_idx[i]].num_occur,fp_out);
        }
    }
    unsigned int bitsRead=0;
    while((ch = fgetc(fp_in)) != EOF) {
        bitsRead+=8;
        for (int i = 0; i < num_leafnintern; i++){            
            if(nd_Huff[nd_idx[i]].type==LEAF && nd_Huff[nd_idx[i]].ch==ch){
                for (int j = 0; j < nd_Huff[nd_idx[i]].lenght; j++){
                    
                 ch_aux = (ch_aux<<1) | ((nd_Huff[nd_idx[i]].code & (1 << j)) >> j);
                // printf("%d ",((nd_Huff[nd_idx[i]].code & (1 << j)) >> j));
                 numBits++;
                 compressedBits++;
                 // 7 porque eu quero que a tabela ASCII VA ate 128 -> 2**7
                 if(numBits==7){
                     fputc(ch_aux,fp_out);
                     numBits=0;
                     ch_aux=0;
                 }
                }
                break;
            }
        }
    }

    if(numBits>0){
        ch_aux=ch_aux<<(7-numBits);
        fputc(ch_aux,fp_out);
        numBits=0;
    }
    // for (int i = 0; i < num_leafnintern; i++){
    //    if(nd_Huff[nd_idx[i]].type==LEAF) printf("\n%d %d",nd_Huff[nd_idx[i]].num_occur,nd_Huff[nd_idx[i]].lenght);
    // }
    
    fseek(fp_out, sizeof(unsigned int), SEEK_SET );
    putw(compressedBits,fp_out);

    printf("\n%d\n%d\n",compressedBits,num_total*8);
    printf("\n\n%d %f\n\n",num_unique,prob);
    fclose(fp_in);
    fclose(fp_out);
}