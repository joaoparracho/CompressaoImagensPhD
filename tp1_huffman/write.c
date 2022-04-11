
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

    
    FILE *fp_in, *fp_out; 
    unsigned int num_unique=10;
    unsigned int compressedBits=10789012;

    unsigned int num_unique_dec;
    unsigned int compressedBits_dec;
    char ch;
    char c[10]={'a','b','c','r','A','C','B','z','.','1'};
    unsigned int num_occur[10]={3992708,61235,60288,54562,51555,26032,5000,450,220,20};

    char c_dec[10];
    


    unsigned int b_x = num_occur[0];
    char numBit=0;
    char numBit_dec;
    while(b_x>0){
        numBit++;
        b_x=b_x/2;
    }
    numBit = (numBit>>3) + (numBit%8!=0);

    fp_in = fopen("write.hf","wb");

    putw(num_unique,fp_in);
    putw(compressedBits,fp_in);
    fputc(numBit,fp_in);
    u_int8_t *ss=(u_int8_t*) calloc(numBit,sizeof(u_int8_t));
    
    for (int i = 0; i < 10; i++){
        fputc(c[i],fp_in);
        for (int j = 0; j < numBit; j++){
            ss[j]=((num_occur[i]>>((numBit-1-j)*8)) & 0x00FF);
        }
        fwrite(ss,numBit,sizeof(u_int8_t),fp_in);        
    }

    for (int i = 65; i < 75; i++){
        fputc(i,fp_in);
    }
    fclose(fp_in);

    

    fp_in = fopen("write.hf","rb");
    fp_out = fopen("write_dec.txt","wb");

    num_unique_dec = getw(fp_in);
    compressedBits_dec = getw(fp_in);
    numBit_dec = fgetc(fp_in);

    unsigned int *num_occur_dec=(unsigned int *) calloc(num_unique_dec,sizeof(unsigned int));

    u_int8_t aux;

    for (int i = 0; i < num_unique; i++){
        if((c_dec[i]  = fgetc(fp_in)) == EOF){
            printf("ERROR - INSERT A VALID .huff FILE");
            return 0;
        }
        for (int j = 0; j < numBit; j++){
            if((aux  = fgetc(fp_in)) == EOF){
                printf("ERROR");
                return 0;
            }
            num_occur_dec[i]=num_occur_dec[i] + (aux<<(numBit-1-j)*8);
        }
    }
    
    while((ch = fgetc(fp_in)) != EOF) {
        fputc(ch,fp_out);
    }


    fclose(fp_in);
    fclose(fp_out);


    


}

