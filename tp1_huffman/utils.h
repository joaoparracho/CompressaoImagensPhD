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

void buildCodes(Node [],int [],int);
void orderNodes(Node [], unsigned int [],unsigned int );
void print_bin(unsigned char , unsigned  );
void print_nodes(Node [],int [],int );
void print_codes(Node [],int [],int );
void buildTree(Node [],int [],int );