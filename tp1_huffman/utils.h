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

void buildCodes(Node [],u_int16_t [],int);
void orderNodes(Node [], u_int16_t [],unsigned int );
void print_bin(unsigned char , unsigned  );
void print_nodes(Node [],u_int16_t [],int );
void print_codes(Node [],u_int16_t [],int );
void buildTree(Node [],u_int16_t [],int );
int isValidNumber(char number[]);