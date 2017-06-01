/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
   included in this distribution in the file called "COPYING". If not,
   see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@eurecom.fr

  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE

 *******************************************************************************/
/* file: ccoding_byte.c
   purpose: Encoding routines for implementing 802.11 convolutionally-coded waveforms
   author: raymond.knopp@eurecom.fr, based on similar code for 3GPP convolutional code (UMTS) by P. Humblet (2000)
   date: 10.2004
*/
//#include "defs.h"
#include "../headers/process.h"
#include "../headers/globalVarINIT.h"


unsigned short gdot11[] = { 0133, 0171 }; // {A,B}
unsigned short gdot11_rev[] = { 0155, 0117 }; // {A,B}
unsigned char  ccodedot11_table[128];
unsigned char  ccodedot11_table_rev[128];


/*************************************************************************

  Encodes for an arbitrary convolutional code of rate 1/2
  with a constraint length of 7 bits.
  The inputs are bit packed in octets (from MSB to LSB).
  Trellis termination is included here
*************************************************************************/



void
ccodedot11_encode (unsigned int numbytes,
		   unsigned char *inPtr,
		   unsigned char *outPtr,
		   unsigned char puncturing)
{
  unsigned int             state;

  unsigned char              c, out, shiftbit =0;

  //  printf("In ccodedot11_encode (%d,%p,%p,%d)\n",numbytes,inPtr,outPtr,puncturing);

#ifdef DEBUG_CCODE
  unsigned int  dummy;
#endif //DEBUG_CCODE
  int bit_index;

  /* The input bit is shifted in position 8 of the state.
     Shiftbit will take values between 1 and 8 */
  state = 0;

#ifdef DEBUG_CCODE
  dummy = 0;
#endif //DEBUG_CCODE

  /* Do not increment inPtr until we read the next octet */
  bit_index=0;
  while (numbytes-- > 0) {
    c = *inPtr++;
#ifdef DEBUG_CCODE
    printf("** %d **\n",c);
#endif //DEBUG_CCODE

    switch (puncturing) {
      case 0:  //rate 1/2
	for (shiftbit = 0; shiftbit<8;shiftbit++) {

	  state >>= 1;
	  if ((c&(1<<shiftbit)) != 0){
	    state |= 64;
	  }

	  out = ccodedot11_table[state];

	  *outPtr++ = out  & 1;
	  *outPtr++ = (out>>1)&1;

#ifdef DEBUG_CCODE
	  printf("%d: %d -> %d (%d)\n",dummy,state,out,ccodedot11_table[state]);
	  dummy+=2;
#endif //DEBUG_CCODE

	}
	break;
      case 1: // rate 3/4
	for (shiftbit = 0; shiftbit<8;shiftbit++) {

	  state >>= 1;
	  if ((c&(1<<shiftbit)) != 0){
	    state |= 64;
	  }

	  out = ccodedot11_table[state];

	  if (bit_index<2)
	    *outPtr++ = out  & 1;
	  if (bit_index!=1)
	    *outPtr++ = (out>>1)&1;

#ifdef DEBUG_CCODE
	  printf("%d: %d -> %d (%d)\n",dummy,state,out,ccodedot11_table[state]);
	  dummy+=2;
#endif //DEBUG_CCODE

	  bit_index=(bit_index==2)?0:(bit_index+1);
	}
	break;
      case 2: // rate 2/3
	for (shiftbit = 0; shiftbit<8;shiftbit++) {

	  state >>= 1;
	  if ((c&(1<<shiftbit)) != 0){
	    state |= 64;
	  }

	  out = ccodedot11_table[state];

	  *outPtr++ = out  & 1;
	  if (bit_index==0)
	    *outPtr++ = (out>>1)&1;

#ifdef DEBUG_CCODE
	  printf("%d: %d -> %d (%d)\n",dummy,state,out,ccodedot11_table[state]);
	  dummy+=2;
#endif //DEBUG_CCODE

	  bit_index=(bit_index==0)?1:0;

	}
	break;
	//Mode-5 for rate 5/6
	case 5:
	for (shiftbit = 0; shiftbit<8;shiftbit++) {

	  state >>= 1;
	  if ((c&(1<<shiftbit)) != 0){
	    state |= 64;
	  }

	  out = ccodedot11_table[state];

	  if (bit_index<2||bit_index==3)
	    *outPtr++ = out  & 1;
	  if (bit_index!=1&&bit_index!=3)
	    *outPtr++ = (out>>1)&1;

#ifdef DEBUG_CCODE
	  printf("%d: %d -> %d (%d)\n",dummy,state,out,ccodedot11_table[state]);
	  dummy+=2;
#endif //DEBUG_CCODE

	  bit_index=(bit_index==4)?0:(bit_index+1);
	}
	break;
      default:
	break;
    }
  }
  /*
  // Termination - Add one NULL byte to terminate trellis
#ifdef DEBUG_CCODE
      printf("Termination\n");
#endif //DEBUG_CCODE
  for (shiftbit = 0; shiftbit<8;shiftbit++) {
    state >>= 1;
    out = ccodedot11_table[state];

    *outPtr++ = out  & 1;
    *outPtr++ = (out>>1)&1;

#ifdef DEBUG_CCODE
    printf("%d: %d -> %d (%d)\n",dummy,state,out,ccodedot11_table[state]);
    dummy+=2;
#endif //DEBUG_CCODE
    printf("%d -> %d (%d)\n",state,out,ccodedot11_table[state]);

  }

  */


}



/*************************************************************************

  Functions to initialize the code tables

*************************************************************************/


/* Basic code table initialization for constraint length 7 */
/* Input in MSB, followed by state in 6 LSBs */

void ccodedot11_init(void)
{
  unsigned int  i, j, k, sum;

  for (i = 0; i < 128; i++) {
    ccodedot11_table[i] = 0;
    /* Compute R output bits */
    for (j = 0; j < 2; j++) {
      sum = 0;
      for (k = 0; k < 7; k++)
        if ((i & gdot11[j]) & (1 << k))
          sum++;
      /* Write the sum modulo 2 in bit j */
      ccodedot11_table[i] |= (sum & 1) << j;
    }
  }
}

/* Input in LSB, followed by state in 6 MSBs */
void ccodedot11_init_inv(void)
{
  unsigned int  i, j, k, sum;

  for (i = 0; i < 128; i++) {
    ccodedot11_table_rev[i] = 0;
    /* Compute R output bits */
    for (j = 0; j < 2; j++) {
      sum = 0;
      for (k = 0; k < 7; k++)
        if ((i & gdot11_rev[j]) & (1 << k))
          sum++;
      /* Write the sum modulo 2 in bit j */
      ccodedot11_table_rev[i] |= (sum & 1) << j;
    }
  }
}

#ifdef OPTIMIZATION
//Optimization for search from the table
/*struct BCCStruct{
unsigned char output[16];
unsigned char endstate;
}BCCTable[256][128];*/
BCCStruct_t BCCTable[256][128];

void init_BCCencode_table()
{
    ccodedot11_init();
    int i;
    unsigned  char  startstate,end =0;
    unsigned  char  shiftbit,out=0;
    unsigned c=0;
    unsigned  char  *dataptr;
    for( i=0;i<256;i++)
    {
      for(startstate=0;startstate<128;startstate++)
      {
         end=startstate;
         dataptr=BCCTable[c][startstate].output;
         for (shiftbit = 0; shiftbit<8;shiftbit++)
         {
	        end>>= 1;
	        if ((c&(1<<shiftbit)) != 0)
	        {
	           end |= 64;
	        }

	     out = ccodedot11_table[end];
         *dataptr++ = out  & 1;
	     *dataptr++ = (out>>1)&1;
         }
         BCCTable[c][startstate].endstate=end;
       }
       c++;
      }

}

void check_BCCcode(unsigned int numbytes,
		   unsigned char *inPtr,
		   unsigned char *outPtr,
		   unsigned char initstate,
		   unsigned char puncturing)
{
    int j;
    unsigned char c;
    int bit_index=0;
    while(numbytes-->0)
    {
      switch (puncturing) {   //mode-0 1/2; mode-1 2/3; mode-2 3/4; mode-3 5/6
      case 0:  //rate 1/2
         c=*inPtr++;
         for(j=0;j<16;j++)
           *outPtr++=BCCTable[c][initstate].output[j];
           initstate=BCCTable[c][initstate].endstate;
      break;
      case 1:   //rate  3/4
         c=*inPtr++;
         for(j=0;j<16;j++)
         {
            if (bit_index<2)
	        *outPtr++ = BCCTable[c][initstate].output[j];
	        j++;
	        if (bit_index!=1)
            *outPtr++ = BCCTable[c][initstate].output[j];
            bit_index=(bit_index==2)?0:(bit_index+1);
         }
         initstate=BCCTable[c][initstate].endstate;
       break;
       case 2:  //rate 2/3
         c=*inPtr++;
         for(j=0;j<16;j++)
         {
            *outPtr++=BCCTable[c][initstate].output[j];
            j++;
            if (bit_index==0)
            *outPtr++ = BCCTable[c][initstate].output[j];
            bit_index=(bit_index==0)?1:0;
         }
         initstate=BCCTable[c][initstate].endstate;
       break;
       case 5:   //rate  5/6
          c=*inPtr++;
          for(j=0;j<16;j++)
          {
             if (bit_index<2||bit_index==3)
	         *outPtr++ = BCCTable[c][initstate].output[j];
	         j++;
	         if (bit_index!=1&&bit_index!=3)
	         *outPtr++ = BCCTable[c][initstate].output[j];
	         bit_index=(bit_index==4)?0:(bit_index+1);
	      }
	      initstate=BCCTable[c][initstate].endstate;
	   break;
       default:
	   break;
       }
    }
}
#endif

/*****************************************************************/
/**
  Test program

******************************************************************/

//#define DEBUG_CCODE
#ifdef DEBUG_CCODE
#include <stdio.h>
#include <stdlib.h>

int main()
{
  //unsigned char test[] = "Thebigredfoxhhh";
  //unsigned char test[]="110100010000000000000000";
  unsigned char* test=(unsigned char*)malloc(1053);
  //unsigned char output[512], *inPtr, *outPtr;
  unsigned char* output=(unsigned char*)malloc(1053*8*2);
  unsigned int i;

  /*test[0] = 0x8B;
  test[1] = 0x00;
  test[2] = 0x00;
  test[3] = 0x7F;
  test[4] = 0x8B;
  test[5] = 0x00;
  test[6] = 0x00;
  test[7] = 0x7F;
  test[8] = 0x8B;
  test[9] = 0x00;
  test[10] = 0x00;
  test[11] = 0x7F;
  test[12] = 0x7F;
  test[13] = 0xFF;
  test[14] = 0xFF;
  //test[0] = 128;
  //test[1] = 0;*/

  for (i = 0; i < 15; i++) printf("%d ", test[i]);
  printf("\n");

  ccodedot11_init();

  unsigned char* inPtr = test;
  unsigned char* outPtr = output;

  //ccodedot11_encode(16, inPtr, outPtr,0);
  //for (i = 0; i < 32; i++) printf("%x ", output[i]);
  ccodedot11_encode(1053, inPtr, outPtr, 5);// mode-0 1/2; mode-1 3/4; mode-2 2/3; mode-5 5/6
  for (i = 0; i < 1053*8*2; i++) printf("%x ", output[i]);
  printf("\n");

  //int fp=fopen("bccDataOut.txt", "w");      /*����һ�������ļ�ֻд*/
  //fputs("Your score is 100", fp);     /*�������ļ�д��һ���ַ�*/
  //fputc(':', fp);                         /*�������ļ�дð��:*/
  //fprintf(fp, "%d\n", i);                 /*�������ļ�дһ������*/
  //for (i = 0; i < 240; i++)
  //    fprintf(fp, "%d\n", output[i]);       /*�������ļ�дһ�ַ���*/
  //fclose(fp);                               /*�ر��ļ�*/
  exit(0);
}
#endif

/** @}*/
