/* MAIN PROGRAM FOR DECODING */

#include <stdio.h>
#include "model.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
   FILE *fp_in, *fp_out;
   if ((fp_in = fopen(argv[1], "rb")) == NULL)
   {
      printf("ERROR: No such file\n");
      return 0;
   }
   fp_out = fopen(argv[2], "wb");

   start_model(); /* Set up other modules		*/
   start_inputing_bits();
   start_decoding(fp_in);

   for (;;) /* Loop through characters	*/
   {
      int ch;
      int symbol;
      symbol = decode_symbol(cum_freq,fp_in); /* decode next symbol 		*/
      if (symbol == EOF_symbol)
         break;                   /* Exit loop if EOF symbol 	*/
      ch = index_to_char[symbol]; /* translate to a character 	*/
      putc(ch, fp_out);           /* write that character 		*/
      update_model(symbol);       /* Update the model 			*/
   }
   exit(0);
}
