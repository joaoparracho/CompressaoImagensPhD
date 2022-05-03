/* MAIN PROGRAM FOR ENCODING */

#include "model.h"

int main(int argc, char *argv[])
{


   FILE *fp_in, *fp_out;

   if (argc > 2)
   {
      printf("Encoding file\n");
   }
   else
   {
      printf("./encode_ppm Inputfile OutputFile\n");
      return 0;
   }

   if ((fp_in = fopen(argv[1], "r")) == NULL)
   {
      printf("ERROR: No such file\n");
      return 0;
   }

   fp_out = fopen(argv[2], "wb");



   start_model(); /* set up other modules.	*/
   start_outputing_bits();
   start_encoding();

   for (;;)
   {
      int ch;
      int symbol;
      ch = getc(fp_in); /* Reads the next character */
      if (ch == EOF)
         break;                           /* Exit loop on end-of-file */
      symbol = char_to_index[ch];         /* Translates to an index 	 */
      encode_symbol(symbol, cum_freq,fp_out);    /* Encode that symbol.	 	 */
      update_model(symbol);               /* Update the model 	 	 */
   }
   encode_symbol(EOF_symbol, cum_freq,fp_out);   /* Encodes the EOF symbol 	 */
   done_encoding(fp_out);                 /* Send the last few bits	 */
   done_outputing_bits(fp_out);
   exit(0);
}
