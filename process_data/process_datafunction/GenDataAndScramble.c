#include<stdlib.h>
#include<string.h>
#include "../../headers/globalVarINIT.h"


void SetN1(unsigned char *pBufa, int n);         //将单字节中某一位置1
void SetN0(unsigned char *pBuf, int n);          //将单字节中某一位清零
unsigned char GetBit(unsigned char buf, int n);  //have been tested

void GenDataAndScramble(unsigned char *data_scramble, int ScrLength, unsigned char *databits, int valid_bits)
{
    int i,j,k;
    int count;
    unsigned char Reg[7]={1,0,1,1,0,1,0};
    unsigned char data[ScrLength];
    memset(data_scramble,0,ScrLength);

    for(i=0;i<2;i++)
    {
    	data[i]=0;
    }

    for(i=2;i<APEP_LEN+2;i++)
    {
		data[i]=databits[i-2];
    }

    //int backlen=(MAC_PAD+N_PAD)/8;

    for(i=APEP_LEN+2;i<(ScrLength);i++)
    {
    	data[i]=0;
    }

    count = 0;
	for(i=0;i<ScrLength;i++)
    {
        //for(j=7;j>=0;j--)
        for(j=0;j<8;j++)        //转换高低位，便于BCC

        {
            count = count + 1;
            if(count<=valid_bits)
            {
                  if((GetBit(data[i],j)+Reg[0]+Reg[3])%2)
                {
                    SetN1(&(data_scramble[i]),j);
                }

                for(k=0;k<6;k++)
                {
                    Reg[k]=Reg[k+1];
                }

                Reg[6]=GetBit(data_scramble[i],j);
            }



        }
    }
}

void SetN1(unsigned char *pBufa, int n)
{
    (*pBufa) |= 1<<n;
}

//将单字节中某一位清零
void SetN0(unsigned char *pBuf, int n)
{
    (*pBuf) &= ~(1<<n);
}

unsigned char GetBit(unsigned char buf, int n)  //have been tested
{
    return (buf>>n) & 0x01;
}
