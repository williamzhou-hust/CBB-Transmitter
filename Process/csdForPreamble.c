#include <math.h>
#include <memory.h>
//#include <stdio.h>
#include <stdlib.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

#define AVX2
#ifndef AVX2
void csdForPreamble(complex32* pStreamBfcsd, complex32* pStreamAfcsd, int NTXindex, int length){
	//printf("Not use AVX2!\n");
    int n=length;
    float phase=-0.00196349541*tcsTableForPreamble(NTXindex);//-2PI*det_F(312.5KHz)*Tcs
    while(n){
         *pStreamAfcsd++=multForComplex32(*pStreamBfcsd++,expComlex32(phase,length/2+1-n));//STF&LTF(n=53)(-26:26),Sig(n=57)(-28:28)
         --n;
    }
}
#else
void initcsdTableForPreamble(){
	//printf("init csdTable For Preamble!\n");
	int NTXindex=0;
	for(NTXindex=0;NTXindex<4;NTXindex++){
	    float phase=-0.00196349541*tcsTableForPreamble(NTXindex);//-2PI*det_F(312.5KHz)*Tcs
		int i=0;
		for(i=0;i<64;i++){
			csdTableForPreamble[NTXindex][i]=expComlex32(phase,i-32);
		}
	}
	csdTableForPreambleFlag=1;
}
void csdForPreamble(complex32* pStreamBfcsd, complex32* pStreamAfcsd, int NTXindex, int length){
	//printf("use AVX2!\n");
	if(!csdTableForPreambleFlag)
		initcsdTableForPreamble();
	complex32 pStreamBfcsd_expan[64];
	memset(pStreamBfcsd_expan,0,64*sizeof(complex32));
	int start=(64-length)/2+1;
	int i=0;
	for(i=start;i<start+length;i++){
		pStreamBfcsd_expan[i].real=pStreamBfcsd[i-start].real;
		pStreamBfcsd_expan[i].imag=pStreamBfcsd[i-start].imag;
	}
	for(i=0;i<4;i++){
		Mult_complex32Vector_2(pStreamBfcsd_expan+i*16,&csdTableForPreamble[NTXindex][i*16],pStreamAfcsd+i*16);
	}
}
#endif


int tcsTableForPreamble(int Ntxindex){//change to tcs
    int index=(N_TX*(N_TX-1))/2+Ntxindex;
    return CyclicShiftTable[index];
}

complex32 expComlex32(float phase, int i){
    complex32 ans;
    ans.real=cos(i*phase)*(0x1<<dotscale);
    ans.imag=sin(i*phase)*(0x1<<dotscale);
    return ans;
}
