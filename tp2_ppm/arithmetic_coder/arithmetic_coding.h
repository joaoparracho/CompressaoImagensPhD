// /* DECLARATIONS USED FOR ARITHMETIC ENCODING AND DECODING */
#include <stdlib.h>
#include <stdio.h>

void start_encoding();
void start_outputing_bits();
void encode_symbol(int,int[],FILE *);
void done_encoding(FILE *);
void done_outputing_bits(FILE *);
void output_bit(int,FILE *);

void start_inputing_bits();
void start_decoding(FILE *fp);
int decode_symbol(int [],FILE *fp);
int input_bit(FILE *fp);


/* SIZE OF ARITHMETIC CODE VALUES */

#define Code_value_bits	22		/* Number of bits in a code value */
typedef long code_value;		/* Type of an arithmetic code value */

#define Top_value 	(((long)1<<Code_value_bits)-1)	/* Largest code value */


/* HALF AND QUARTER POINTS IN THE CODE VALUE RANGE */

#define	First_qtr	(Top_value/4+1)	/* Point after first quarter 	*/
#define Half		(2*First_qtr)	/* Point after first half 		*/
#define Third_qtr 	(3*First_qtr)	/* Point after third quarter 	*/


