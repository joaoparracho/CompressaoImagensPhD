#include "utils.h"

void buildCodes(Node nd_Huff[],u_int16_t nd_idx[],int num_leafnintern){

    for (int i = 0; i < num_leafnintern; i++){
        if(nd_Huff[nd_idx[i]].type==INTERNAL){
            if(nd_Huff[nd_idx[i]].right!=NULL){
                nd_Huff[nd_idx[i]].right->code = nd_Huff[nd_idx[i]].code + (1<<nd_Huff[nd_idx[i]].lenght);
                nd_Huff[nd_idx[i]].right->lenght=nd_Huff[nd_idx[i]].lenght+1;
            }
            if(nd_Huff[nd_idx[i]].left!=NULL){
               nd_Huff[nd_idx[i]].left->code = nd_Huff[nd_idx[i]].code;
               nd_Huff[nd_idx[i]].left->lenght=nd_Huff[nd_idx[i]].lenght+1;
            }
        }
    }

}
void orderNodes(Node node[], u_int16_t idx[],unsigned int len){
    Node nodeAux;
    unsigned int idxAux=0;
    for (int i = len-1; i >= 0; i--){
        for (int j = i-1; j >= 0; j--){
            if(node[idx[i]].num_occur>node[idx[j]].num_occur ){
                idxAux=idx[i];
                idx[i]=idx[j];
                idx[j]=idxAux;
            }
            if(node[idx[i]].num_occur==node[idx[j]].num_occur&& (node[idx[i]].type==INTERNAL) && ( (node[idx[j]].type==LEAF) )){
                idxAux=idx[i];
                idx[i]=idx[j];
                idx[j]=idxAux;

            }
            
        }
    }    
}

void print_bin(unsigned char value, unsigned int numChar){
    for (int i = sizeof(char) * numChar; i >= 0; i--)
        printf("%d", (value & (1 << i)) >> i );
    putc('\n', stdout);
}
void print_nodes(Node n[],u_int16_t idx[],int num_elem){
    printf("\n=================\n");
    for (int j = 0; j < num_elem; j++){
        printf(" %c - ",n[idx[j]].ch);
        if(n[idx[j]].left!=NULL) printf(" %c", n[idx[j]].left->ch);
        if(n[idx[j]].right!=NULL) printf(" , %c",n[idx[j]].right->ch);
        printf("\n");
    }
}
void print_codes(Node n[],u_int16_t idx[],int num_elem){
    printf("\n====================================\n");
    for (int i = 0; i < num_elem; i++){
        if(n[idx[i]].type==LEAF){
            printf("%c \t   %d   \t",n[idx[i]].ch,n[idx[i]].num_occur);
            print_bin(n[idx[i]].code,n[idx[i]].lenght-1);
        }
    }
}
void buildTree(Node n[],u_int16_t nd_idx[],int num_unique){
    unsigned int num_leafnintern = (num_unique<<1)-1;
    unsigned int id1=num_unique-1;
    unsigned int id2=id1;
    for (int i = 0; i < num_unique-1; i++){
        id1=num_unique-1-i;
        id2=num_unique+i;
        n[nd_idx[id2]].left=&n[nd_idx[id1]];
        n[nd_idx[id2]].right=&n[nd_idx[id1-1]];
        n[nd_idx[id2]].type=INTERNAL;
        n[nd_idx[id2]].ch='z';
        n[nd_idx[id2]].num_occur=n[nd_idx[id1]].num_occur+n[nd_idx[id1-1]].num_occur;

        orderNodes(n,nd_idx,num_leafnintern);

        // for (int j = 0; j < num_leafnintern; j++){
        //     printf("\n %c - %d",n[nd_idx[j]].ch,n[nd_idx[j]].num_occur);
        // }
        // printf("\n");   
    }
}
int isValidNumber(char number[]){
    int i=0;
    if(number[0]=='-') return 0;
    for(;number[i]!=0;i++){
        if(number[i]>'9' || number[i]<'0'){
            return 0;
        }
    }
    return 1;
    
}