// /* DECLARATIONS USED FOR ARITHMETIC ENCODING AND DECODING */
#include <stdlib.h>
#include <stdio.h>
typedef long code_value;		/* Type of an arithmetic code value */

void start_encoding(code_value* low, code_value* high);
void start_outputing_bits();
void encode_symbol(FILE *,code_value* low, code_value* high);
void done_encoding(FILE *,code_value);
void done_outputing_bits(FILE *);
void output_bit(int,FILE *);

void start_inputing_bits();
void start_decoding(FILE *fp);
int decode_symbol(int [],FILE *fp);
int input_bit(FILE *fp);


/* SIZE OF ARITHMETIC CODE VALUES */

#define Code_value_bits	31		/* Number of bits in a code value */

#define Top_value 	(((long)1<<Code_value_bits)-1)	/* Largest code value */

static long bits_to_follow;  /* Number of opposite bits to output after	*/


/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE */

#define	First_qtr	(Top_value/4+1)	/* Point after first quarter 	*/
#define Half		(2*First_qtr)	/* Point after first half 		*/
#define Third_qtr 	(3*First_qtr)	/* Point after third quarter 	*/


