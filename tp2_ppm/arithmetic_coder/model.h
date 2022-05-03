/* INTERFACE TO THE MODEL */


#include "arithmetic_coding.h"
#include <stdlib.h>
#include <stdio.h>

void start_model();	
void update_model(int);

/* THE SET OF SYMBOLS THAT MAY BE ENCODED */

#define No_of_chars	128			            /* Number of character symbols */
#define EOF_symbol	(No_of_chars + 1)	    /* Index of EOF symbol	*/

#define No_of_symbols 	(No_of_chars + 1)	/* Total number of symbols */


/* TRANSLATION TABLES BETWEEN CHARACTERS AND SYMBOL INDEXES */

int char_to_index[No_of_chars];			       /* To index from charater */
unsigned char index_to_char[No_of_symbols+1];  /* To character from index */


/* CUMULATIVE FREQUENCY TABLE */

#define Max_frequency	1048575			/* Maximum allowed frequency count 	*/
						                /* 2^14 -1 	            			*/
int cum_freq[No_of_symbols+1];			/* Cummulative symbol frequencies 	*/

