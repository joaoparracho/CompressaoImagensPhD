#include "utils.h"


int main(int argc, char* argv[]){

    FILE *fp_in, *fp_out;
    Node *nd_Huff_dec;
    Node *node_aux;

    u_int8_t num_unique=0;
    char ch=0;
    char mv=0;
    u_int8_t numBits=0;
    u_int8_t bit;
    u_int8_t code=0;
    u_int8_t numBits_dec=0;
    
    u_int16_t num_leafnintern=0;
    u_int16_t *idx;

    u_int32_t huffBits=0;
    u_int32_t totalBits=0;
    

    if(argc == 3){
        printf("Decompressing file\n");
    }
    else {
        printf("./encode Inputfile OutputFile\n");
        return 0;
    }

    if((fp_in = fopen(argv[1],"rb"))==NULL){
		printf("ERROR: No such file\n");
		return 0;
	}
    fp_out = fopen(argv[2],"wb");

    if((num_unique = getc(fp_in)) == EOF) {
        printf("ERROR - INSERT A VALID .huff FILE");
        return 0;
    }
    if((numBits = getc(fp_in)) == EOF) {
        printf("ERROR - INSERT A VALID .huff FILE");
        return 0;
    }
   
    if((huffBits = getw(fp_in)) == EOF) {
        printf("ERROR - INSERT A VALID .huff FILE");
        return 0;
    }
    if((mv = getc(fp_in)) == EOF) {
        printf("ERROR - INSERT A VALID .huff FILE");
        return 0;
    }
    printf("\nREAD %d bytes\n",huffBits);
    
    num_leafnintern=(num_unique<<1)-1;
    nd_Huff_dec = (Node *)calloc(num_leafnintern,sizeof(Node));
    idx = (u_int16_t *)calloc(num_leafnintern,sizeof(u_int16_t));
    
    for (u_int8_t i = 0; i < num_unique; i++){
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
    for (u_int16_t i = num_unique; i < num_leafnintern; i++){
        idx[i]=i;
    }

    buildTree(nd_Huff_dec,idx, num_unique);

    buildCodes(nd_Huff_dec,idx,num_leafnintern);

    node_aux=&nd_Huff_dec[idx[0]];
    int bitsTofile=0;
    char i;
    while(totalBits<huffBits) {
        ch = fgetc(fp_in);
        if(totalBits+numBits==totalBits){
            i=numBits;
        }
        else{
            i=7;
        }
        for (; i >= 0; i--){
            
            bit=((ch & (1 << i)) >> i );
            //printf("%d ",bit);
            numBits_dec++;
            totalBits++;
            

            if(bit){
                if(node_aux->right->type == LEAF){
                    fputc(node_aux->right->ch,fp_out);
                    //totalBits+=node_aux->right->lenght;
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
                    //totalBits+=node_aux->left->lenght;
                    node_aux=&nd_Huff_dec[idx[0]];
                    numBits_dec=0;
                }
                else{
                    node_aux=(node_aux->left);
                }

            }
            if(totalBits==huffBits){
                printf("\nFile Decoded\n");
                break;
            }
        }
        //printf("\n");
        if(totalBits==huffBits){
            printf("\nFile Decoded\n");
            break;
        }
    }
    

    fclose(fp_in);
    fclose(fp_out);
}