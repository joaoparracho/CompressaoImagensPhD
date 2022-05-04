/* ARITHMETIC ENCODING ALGORITHM.	*/

#include "arithmetic_coding.h"

static void bit_plus_follow(); /* Routine that follows	*/

/* CURRENT STATE OF THE ENCODING */


                             /* the next bit.				            */

/* START ENCODING A STREAM OF SYMBOLS */

void start_encoding(code_value* low, code_value* high)
{
  *low = 0; /* Full code range */
  *high = Top_value;
  bits_to_follow = 0; /* No bits to follow next */
}

/* ENCODE A SYMBOL */

void encode_symbol(fp,low,high) 
FILE *fp;
code_value* low;
code_value* high;
{
  
/* symbol           */
  for (;;)
  { /* Loop to output bits	*/
    if (*high < Half)
    {
      bit_plus_follow(0, fp); /* Output 0 if in low half */
    }
    else if (*low >= Half)
    { /* Output 1 if in high half	*/
      bit_plus_follow(1, fp);
      (*low) -= Half;
      (*high) -= Half; /* Subtract offset to top */
    }
    else if ((*low) >= First_qtr && (*high) < Third_qtr)
    { /* Output an opposite bit   */
      /* later if in middle half.	*/
      bits_to_follow += 1;
      (*low) -= First_qtr; /* Subtract offset to middle */
      (*high) -= First_qtr;
    }
    else
      break; /* Otherwise exit loop */
    if((*low)>Half)
      printf("Oii");
    (*low) = 2 * (*low);
    (*high) = 2 * (*high + 1); /* Scale up code range */
  }
}

/* FINISH ENCODING THE STREAM */

void done_encoding(FILE *fp,code_value low)
{
  bits_to_follow += 1; /* Output two bits that 		*/
  if (low < First_qtr)
    bit_plus_follow(0, fp); /* select the quarter that 		*/
  else
    bit_plus_follow(1, fp); /* the current code range		*/
} /* contains						*/

/* OUTPUT BITS PLUS THE FOLLOWING OPPOSITE BITS */

static void bit_plus_follow(bit, fp) int bit;
FILE *fp;
{
  output_bit(bit, fp); /* Output the bit				*/
  while (bits_to_follow > 0)
  {
    output_bit(!bit, fp); /* Ouput bits_to_follow			*/
    bits_to_follow -= 1;  /* opposite bits. Set			*/
  }                       /* bits_to_follow to zero		*/
}
