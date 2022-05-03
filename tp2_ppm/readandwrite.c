/* MAIN PROGRAM FOR ENCODING */
#include <stdlib.h>
#include <stdio.h>

int readFile(FILE **fp, char filename[]){
    if((*fp = fopen(filename,"r"))==NULL){
		printf("ERROR: No such file\n");
		return 0;
	}
}

int openFile(FILE **fp, char filename[]){
    if((*fp = fopen(filename,"wb"))==NULL){
		printf("ERROR: No such file\n");
		return 0;
	}
}

void writeCharacterToFile(FILE *fp,char ch){
    putc(ch,fp);
}

void readAndPrintCharacterToFile(FILE *fp){
    printf("%c",getc(fp));
}

int main(int argc, char* argv[])
{  

    FILE *fp_in, *fp_out;

    if(argc > 2 ){
        printf("Encoding file\n");
    }
    else {
        printf("./encode_ppm Inputfile OutputFile\n");
        return 0;
    }

    readFile(&fp_in, argv[1]);
    openFile(&fp_out, argv[2]);

    writeCharacterToFile(fp_out,'r');
    writeCharacterToFile(fp_out,'d');
    writeCharacterToFile(fp_out,'e');

    readAndPrintCharacterToFile(fp_in);
    readAndPrintCharacterToFile(fp_in);
    readAndPrintCharacterToFile(fp_in);
    readAndPrintCharacterToFile(fp_in);

    return 0;
}


