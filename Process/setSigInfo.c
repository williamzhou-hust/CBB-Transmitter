#include "../headers/process.h"

void setSigInfo(unsigned char* SigInfo, int length){
     //the SigInfo contains the data below
     /*Rate = [1 1 0 1];
     Reserved = 0;
     Length = [0 0 1 0 0 0 0 0 0 0 0 0];
     Parity = 0;
     Tail = [0 0 0 0 0 0];
     SigInfo = [Rate, Reserved, Length, Parity, Tail];*/
     SigInfo[0]=0x8B;
     SigInfo[1]=0x00;
     SigInfo[2]=0x00;
}
