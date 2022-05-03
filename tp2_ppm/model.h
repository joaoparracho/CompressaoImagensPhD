#include <stdlib.h>
#include <stdio.h>

#define MAX_ORDER 18

static u_int8_t NUM_CHARS=42;
static u_long NUM_NODES=80000000;

struct node {
		u_int8_t symbol;
        unsigned long *right;        /* indx to the next node on the same level */ 
		u_int8_t num_descendant;
        unsigned long num_escapes;
		unsigned long cum_count;          /* symbol cumulative count */
        unsigned long total_count;          /* symbol cumulative count */
		long  down_node;          /* idx pointer to next level */
		long  next_context;       /* idx the vine pointer */
};

typedef struct node Node;

void set_node(Node *node, int16_t _symbol, unsigned long _cum_count, long _down, long _vine,unsigned long _num_escapes,unsigned long total_count,u_int8_t _num_descendant);

void buildTrie(Node *node);

int add_symbol(Node *trie,long *down_node, long* next_context, int16_t symbol, unsigned long *depth,unsigned long *idx_nodes,int *searching,unsigned long *idx_next_context,int *a);

long search_symbol(Node *trie,long next_context, u_int8_t symbol);