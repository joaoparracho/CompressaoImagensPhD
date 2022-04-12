#include "utils.h"


int main(int argc, char* argv[]){

    FILE *fp_in, *fp_out;

    Node *nd_Huff_dec;
    Node *nd_Huff = (Node*) calloc(ASCII_SIZE,sizeof(Node));

    char ch;
    u_int8_t num_unique=0;
    u_int8_t max_code=8;
    u_int8_t numBits=0;
    u_int8_t verbose=0;
    u_int8_t mv=0;
    u_int8_t ch_aux=0;
    
    u_int16_t *nd_idx = (u_int16_t *) calloc(ASCII_SIZE,sizeof(u_int16_t));
    u_int16_t num_leafnintern=0;
    u_int16_t id1=0;
    u_int16_t id2=0;
        
    u_int32_t num_total=0;
    u_int32_t bytesW=0;
    u_int32_t compressedBits=0;

    float prob=0;
    
    if(argc > 2  && argc<6){
        printf("Encoding file\n");
        if(argc>3){
            if(strcmp(argv[3],"-v")==0) verbose =1;
            else if (strcmp(argv[3],"-m")==0)  mv=1;
            if(argc>4){
                if(strcmp(argv[4],"-v")==0) verbose =1;
                else if (strcmp(argv[4],"-m")==0)  mv=1;
            }
        }
    }
    else {
        printf("./encode Inputfile OutputFile -v -m\n");
        printf("-v \t Verbose (default:0)\n-m \t Use Minimum Variance Huffman Codes (default:0)");
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
    
    orderNodes(nd_Huff,nd_idx,ASCII_SIZE,0);
   
    num_leafnintern=(num_unique<<1)-1;

    if(num_leafnintern>ASCII_SIZE){
        nd_Huff = (Node *)realloc(nd_Huff,(num_leafnintern-ASCII_SIZE)*sizeof(Node));
        nd_idx = (u_int16_t *)realloc(nd_idx,(num_leafnintern-ASCII_SIZE)*sizeof(u_int16_t));
        for (u_int16_t i = ASCII_SIZE; i <num_leafnintern ; i++){
            nd_idx[i]=i;
        }
    }

    buildTree(nd_Huff,nd_idx, num_unique,mv);

    buildCodes(nd_Huff,nd_idx,num_leafnintern);
    
    fseek(fp_in, 0, SEEK_SET); //rewind the text file to the beggining
    fputc(num_unique,fp_out);
    fputc(numBits,fp_out);
    putw(compressedBits,fp_out);
    fputc(mv,fp_out);
    
    for(int i = 0; i < num_leafnintern; i++){   
        if(nd_Huff[nd_idx[i]].type==LEAF){
            fputc(nd_Huff[nd_idx[i]].ch,fp_out);
            putw(nd_Huff[nd_idx[i]].num_occur,fp_out);
        }
    }
    
    while((ch = fgetc(fp_in)) != EOF) {
        
        for (u_int16_t i = 0; i < num_leafnintern; i++){            
            if(nd_Huff[nd_idx[i]].type==LEAF && nd_Huff[nd_idx[i]].ch==ch){
                for (int j = 0; j < nd_Huff[nd_idx[i]].lenght; j++){
                 ch_aux = (ch_aux<<1) | ((nd_Huff[nd_idx[i]].code & (1 << j)) >> j);
                 numBits++;
                 compressedBits++;
                 if(numBits==8){
                     bytesW++;
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
        ch_aux=ch_aux<<(8-numBits);
        fputc(ch_aux,fp_out);
        bytesW++;
    }
    
    fseek(fp_out, sizeof(num_unique), SEEK_SET );
    fputc(numBits,fp_out);
    putw(compressedBits,fp_out);
    if(verbose){
        printf("\n\nNumber of unique symbols: %d symbols\n",num_unique);
        print_codes(nd_Huff,nd_idx,num_leafnintern);
        printf("\nInput file size: %d bytes\n",num_total);
        printf("Encoded stream size (without header): %d bytes\n",bytesW);
        printf("Encoded header size: %d bytes\n",num_unique*(4+1)+7);
        printf("Encoded stream size (with header): %d bytes\n",bytesW + num_unique*(4+1)+7);
        printf("Compression rate: %.02f \n",(float)(num_total/(float)(bytesW + num_unique*(4+1)+7)));
        printf("Data rate saving: %.02f %% \n",(float)(1-(float)(bytesW + num_unique*(4+1)+7)/num_total)*100);

        float average_len=0.0;
        printf("Average Lenght of Input file: 8 bits/symbol\n");//%.02f\n",average_len);
        average_len=0.0;
        for(int i=0;i<num_leafnintern;i++){
            if(nd_Huff[nd_idx[i]].type==LEAF){
                average_len +=  (float)nd_Huff[nd_idx[i]].num_occur*nd_Huff[nd_idx[i]].lenght/num_total;
            }
        }
        printf("Average Lenght of Encoded stream: %.02f bits/symbol\n\n",average_len);
    }

    
    if((bytesW + num_unique*(4+1)+6)>num_total){
        printf("Huffman compression is not efficient for the input file.\nThe encoded result is larger than the input!!\n");
    }
    fclose(fp_in);
    fclose(fp_out);
}