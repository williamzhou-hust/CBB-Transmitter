#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../../headers/globalVarINIT.h"


void SetN1(unsigned char *pBufa, int n);         //将单字节中某一位置1
void SetN0(unsigned char *pBuf, int n);          //将单字节中某一位清零
unsigned char GetBit(unsigned char buf, int n);  //have been tested

#ifdef OPTIMIZATION
Scramble_t ChartTable[256][128];
void Creatnewchart()
{
	//printf("use OPT scramble!\n");
	void __GenDataAndScramble_aux(unsigned char *data_scramble, unsigned char databits, unsigned char Reg,unsigned char *Reg_out);
    unsigned char i,j;
    i=0;
    int k;
    for(k=0;k<256;k++)
    {
       for(j=0;j<128;j++)
       {
          __GenDataAndScramble_aux(&(ChartTable[i][j].data_out),i,j,&(ChartTable[i][j].reg_out));
       }
       i++;
    }
}
void __GenDataAndScramble_aux(unsigned char *data_scramble, unsigned char databits, unsigned char Reg,unsigned char *Reg_out)
{
    int i,j;
	unsigned char regg[7];//由字节获取7位数组
	for(i=0;i<7;i++)
	{
		regg[i]=GetBit(Reg,i);
	}

    for(i=0;i<8;i++)
    {
        if((GetBit(databits,i)+regg[0]+regg[3])%2)
        {
            SetN1(data_scramble,i);
        }
        for(j=0;j<6;j++)
        {
            regg[j]=regg[j+1];
        }
        regg[6]=GetBit(*data_scramble,i);
    }
    *Reg_out=64*regg[6]+32*regg[5]+16*regg[4]+8*regg[3]+4*regg[2]+2*regg[1]+regg[0];
}
#endif

void GenDataAndScramble(unsigned char *data_scramble, int ScrLength, unsigned char *databits, int valid_bits)
{
    int i,j,k;
    int count;
    unsigned char Reg[7]={1,0,1,1,0,1,0};
	#ifdef OPTIMIZATION
	unsigned char reg_initstate=45;
	#endif
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
		#ifndef OPTIMIZATION
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
		#else
		data_scramble[i]=ChartTable[data[i]][reg_initstate].data_out;
		reg_initstate=ChartTable[data[i]][reg_initstate].reg_out;
		#endif
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
