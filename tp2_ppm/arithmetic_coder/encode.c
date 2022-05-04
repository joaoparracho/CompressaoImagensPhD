/* MAIN PROGRAM FOR ENCODING */

#include "model.h"
#include "model_racho.h"
#include <math.h>

int main(int argc, char *argv[])
{


   FILE *fp_in, *fp_out;
   long range;
   static code_value low, high; /* ends of the current code-region */

   unsigned long depth=0;
	int16_t num_escapes=0;
	int searching=0;
	unsigned long idx_next_context=0;
	//index 0 reserved to ROOT
	unsigned long idx_nodes=1;
	long down_node;
	long next_context;
   static Node *trie;
	trie = (Node*) malloc(NUM_NODES*sizeof(Node));
	//Total Count a 1 por causa do <ESC>
	set_node(&trie[0],0,0,0,0,0,1,0);



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
   int ch;
   int nd_idx[No_of_chars+1]={0};
   int num_unique=0;

   // while((ch = fgetc(fp_in)) != EOF) { 
   //      nd_idx[(int)ch]=1;
   //  } 

   //  for (int i = 0; i < No_of_chars; i++){
   //      if(nd_idx[i]){
   //         nd_idx[num_unique++]=i+1;
   //      }
   //  }
   // nd_idx[num_unique]=No_of_chars;
   // fseek(fp_in, 0, SEEK_SET);
   //start_model_racho(num_unique,nd_idx);
   start_model(); /* set up other modules.	*/
   start_outputing_bits();
   start_encoding(&low,&high);
   long i=0;
   
   // int symbol;
   // fputc(num_unique,fp_out);
   // for (int j = 0; j < num_unique; j++)
   // {
   //    range = (long)(high - low) + 1; 
   //    high = low + (range * cum_freq[nd_idx[j]]) / cum_freq[0] - 1; /* region to that   */
   //    low = low + (range * cum_freq[nd_idx[j+1]]) / cum_freq[0];          /* allotted to this */
   //    encode_symbol(fp_out,&low,&high);    /* Encode that symbol.	 	 */
   //    update_model(symbol);   
   // }
   
              

   for (;;)
   {
      int symbol;
      int ch;
      
      ch = getc(fp_in); /* Reads the next character */
      
      if (ch == EOF)
         break;                           /* Exit loop on end-of-file */
      //else printf("%c\n",ch);                                                         /* size of the current_code region	*/
      range = (long)(high - low) + 1;                                /* Narrow the code  */
      
      while(!add_symbol(trie,&down_node, &next_context, ch, &depth,&idx_nodes,&searching,&idx_next_context,&num_escapes,&high,&low,range)){
         encode_symbol(fp_out,&low,&high);
         range = (long)(high - low) + 1;
         //printf("<ESC>%c %lu %lu\n",ch,high,low);
         num_escapes=0;
	   }
      encode_symbol(fp_out,&low,&high);    /* Encode that symbol.	 	 */
      if(num_escapes==1){
         num_escapes=0;
         //codifica o ultimo <esc>
         //printf("<ESC>%c %lu %lu\n",ch,high,low);
         //codifica ordem -1
         range = (long)(high - low) + 1;

         high = low + (range * (ch+1)) / cum_freq[0] - 1; /* region to that   */
         low = low + (range * ch) / cum_freq[0]; 
         encode_symbol(fp_out,&low,&high);    /* Encode that symbol.	 	 */
         printf("%c %lu %lu\n",ch,high,low);
         update_model(symbol);               /* Update the model 	 	 */
      }
   }

   range = (long)(high - low) + 1;              /* Narrow the code  */
   high = low + round((float)(range * 1) / cum_freq[0]) - 1;  /* region to that */
   low = low + round((float)(range * 0) / cum_freq[0]); 
   encode_symbol(fp_out,&low,&high);            /* Encodes the EOF symbol 	 */
   done_encoding(fp_out,low);                   /* Send the last few bits	 */
   done_outputing_bits(fp_out);
   exit(0);
}
