#include <stdlib.h>
#include <stdio.h>

/* SIZE OF ARITHMETIC CODE VALUES */

#define Code_value_bits	22		/* Number of bits in a code value */
typedef long code_value;		/* Type of an arithmetic code value */

#define Top_value 	(((long)1<<Code_value_bits)-1)	/* Largest code value */

/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE */
#define	First_qtr	(Top_value/4+1)	/* Point after first quarter 	*/
#define Half		(2*First_qtr)	/* Point after first half 		*/
#define Third_qtr 	(3*First_qtr)	/* Point after third quarter 	*/

#define MAX_ORDER 2

static u_long NUM_NODES=150000000;

struct node {
		u_int8_t symbol;
		unsigned long last_node_added;
		unsigned long r;
		u_int8_t num_descendant;
        unsigned long first_node_added;
		unsigned long cum_count;    /* symbol cumulative count */
        unsigned long total_count;  /* symbol cumulative count */
		long  down_node;          	/* idx pointer to next level */
		long  next_context;       	/* idx the vine pointer */
};





typedef struct node Node;

void set_node(Node *node, int16_t _symbol, unsigned long _cum_count, long _down, long _vine,unsigned long _num_escapes,unsigned long total_count,u_int8_t _num_descendant);

void buildTrie(Node *trie,long *down_node, long* next_context, int16_t symbol, unsigned long *depth,unsigned long *idx_nodes,int *searching,unsigned long *idx_next_context);

int add_symbol(Node *trie,long *down_node, long* next_context, int16_t symbol, unsigned long *depth,unsigned long *idx_nodes,int *searching,unsigned long *idx_next_context,int16_t *num_escapes,long *high,long* low, long range);

long search_symbol(Node *trie,long next_context, u_int8_t symbol);

long search_left_symbol(Node *trie,long index, u_int8_t symbol);

void update_CumCount(Node *trie,long index);
void update_CumCount_context(Node *trie,long index);

void print_right(Node *trie,long index);