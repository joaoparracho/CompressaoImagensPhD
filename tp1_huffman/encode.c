#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASCII_SIZE 255
#define LEAF 0b00000000
#define INTERNAL 0b00000001
#define NSORT 0b00000100

struct node{
    char ch;
    unsigned int code;
    unsigned int lenght;
    unsigned int num_occur;
    char type; //xxxxxxx0 leaf; xxxxxxx1 internal; xxxxx0xx can be sorted; xxxxx1xx cannot be sorted;
    struct node *left,*right;
};

typedef struct node Node;


void buildCodes(Node nd_Huff[],int nd_idx[],int num_leafnintern){

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
void orderNodes(Node node[], unsigned int idx[],unsigned int len){
    Node nodeAux;
    unsigned int idxAux=0;
    for (int i = len-1; i >= 0; i--){
        for (int j = i-1; j >= 0; j--){
            if(node[idx[i]].num_occur>=node[idx[j]].num_occur && (node[idx[i]].num_occur==node[idx[j]].num_occur && node[idx[i]].type==LEAF)==0){
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
void print_nodes(Node n[],int idx[],int num_elem){
    printf("\n=================\n");
    for (int j = 0; j < num_elem; j++){
        printf(" %c - ",n[idx[j]].ch);
        if(n[idx[j]].left!=NULL) printf(" %c", n[idx[j]].left->ch);
        if(n[idx[j]].right!=NULL) printf(" , %c",n[idx[j]].right->ch);
        printf("\n");
    }
}
void print_codes(Node n[],int idx[],int num_elem){
    printf("\n=============\n");
    for (int i = 0; i < num_elem; i++){
        //if(n[idx[i]].type==LEAF){
            printf("%c - ",n[idx[i]].ch);
            print_bin(n[idx[i]].code,n[idx[i]].lenght-1);
        //}
    }
}
void buildTree(Node n[],int nd_idx[],int num_unique){
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

        // for (int i = 0; i < num_leafnintern; i++){
        //     printf("\n %c - %d",n[nd_idx[i]].ch,n[nd_idx[i]].num_occur);
        // }
        // printf("\n");
    }

}

int main(int argc, char* argv[]){

    FILE *fp_in, *fp_out;

    Node *nd_Huff_dec;
    Node *nd_Huff = (Node*) calloc(ASCII_SIZE,sizeof(Node));

    unsigned int *nd_idx = (unsigned int*) calloc(ASCII_SIZE,sizeof(unsigned int));
    unsigned int num_unique=0;
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

    //print_codes(nd_Huff,nd_idx,num_leafnintern);

    for (int i = 0; i < num_leafnintern; i++){
        printf(" %c - %d - %0.5f \n",nd_Huff[nd_idx[i]].ch,nd_Huff[nd_idx[i]].num_occur,(float)nd_Huff[nd_idx[i]].num_occur/num_total);
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

    

  //////////////////////////////////////////DECODER//////////////////////////////////////////////////////
    int huffBits=0;

    if((fp_in = fopen(argv[2],"rb"))==NULL){
		printf("ERROR: No such file\n");
		return 0;
	}
    fp_out = fopen(argv[3],"wb");

    if((num_unique = getw(fp_in)) == EOF) {
        printf("ERROR - INSERT A VALID .huff FILE");
        return 0;
    }
    if((huffBits = getw(fp_in)) == EOF) {
        printf("ERROR - INSERT A VALID .huff FILE");
        return 0;
    }
    printf("\nREAD %d bits\n",huffBits);

    num_leafnintern=(num_unique<<1)-1;
    nd_Huff_dec = (Node *)calloc(num_leafnintern,sizeof(Node));
    int *idx = (int *)calloc(num_leafnintern,sizeof(int));
    Node *node_aux;
    

    for (int i = 0; i < num_unique; i++){
        if((nd_Huff_dec[i].ch  = fgetc(fp_in)) == EOF){
            printf("ERROR - INSERT A VALID .huff FILE");
            return 0;
        }
        if((nd_Huff_dec[i].num_occur = getw(fp_in)) == EOF) {
            printf("ERROR - INSERT A VALID .huff FILE");
            return 0;
        }
        idx[i]=i;
    }
    for (int i = num_unique; i < num_leafnintern; i++){
        idx[i]=i;
    }

    buildTree(nd_Huff_dec,idx, num_unique);

    //print_nodes(nd_Huff_dec,idx,num_leafnintern);

    buildCodes(nd_Huff_dec,idx,num_leafnintern);

    //print_codes(nd_Huff_dec,idx,num_leafnintern);

    node_aux=&nd_Huff_dec[idx[0]];
    char bit;
    char code=0;
    char numBits_dec=0;
    unsigned int totalBits=0;
    
    while((ch = fgetc(fp_in)) != EOF) {
        for (char i = 6; i >= 0; i--){

            if(totalBits==huffBits){
                printf("\nFile Decoded\n");
                break;
            }
            
            bit=((ch & (1 << i)) >> i );
            //printf("%d ",bit);
            numBits_dec++;
            code = code | (bit<<numBits_dec) ;

            if(bit){
                if(node_aux->right->type == LEAF){
                    fputc(node_aux->right->ch,fp_out);
                    totalBits+=node_aux->right->lenght;
                    node_aux=&nd_Huff_dec[idx[0]];
                    numBits_dec=0;
                }
                else{
                    node_aux=node_aux->right;
                }
            }
            else{
                if(node_aux->left->type == LEAF){
                    fputc(node_aux->left->ch,fp_out);
                    totalBits+=node_aux->left->lenght;
                    node_aux=&nd_Huff_dec[idx[0]];
                    numBits_dec=0;
                }
                else{
                    node_aux=(node_aux->left);
                }

            }
        }
        if(totalBits==huffBits){
            printf("\nFile Decoded\n");
            break;
        }
    
    }
    

    fclose(fp_in);
    fclose(fp_out);
}