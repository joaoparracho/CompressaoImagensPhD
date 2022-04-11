#include "utils.h"


int main(int argc, char* argv[]){

    FILE *fp_in, *fp_out;

    Node *nd_Huff_dec;
    int huffBits=0;
    unsigned int num_unique=0;
    unsigned int num_leafnintern=0;
    char ch=0;

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