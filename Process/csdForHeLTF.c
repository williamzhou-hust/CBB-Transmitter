#include <math.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

#define AVX2
#ifndef AVX2
void csdForHeLTF(complex32* pStreamBfcsd, complex32* pStreamAfcsd, int NTXindex, int length){
     int n=length;
     //-2PI*det_F(312.5KHz/4)*Tcs -0.00196349541/4
     float phase=-0.00049087385*shiftTableForHeLTF[N_TX-1][NTXindex];
     while(n){
          //STF&LTF(n=53)(-26:26),Sig(n=57)(-28:28) HELTF -128:127
          *pStreamAfcsd++=multForComplex32(*pStreamBfcsd++,expComlex32(phase,length/2-n));
          --n;
     }
}
#else
void initcsdTableForHeLTF(){
	//printf("init csdTable For Preamble!\n");
	int NTXindex=0;
	for(NTXindex=0;NTXindex<4;NTXindex++){
		float phase=-0.00049087385*shiftTableForHeLTF[N_TX-1][NTXindex];
		int i=0;
		for(i=0;i<256;i++){
			csdTableForHeLTF[NTXindex][i]=expComlex32(phase,i-128);
		}
	}
	csdTableForHeLTFFlag=1;
}
void csdForHeLTF(complex32* pStreamBfcsd, complex32* pStreamAfcsd, int NTXindex, int length){
	//printf("use AVX2!\n");
	if(!csdTableForHeLTFFlag)
		initcsdTableForHeLTF();
	int i=0;
	for(i=0;i<16;i++){
		Mult_complex32Vector_2(pStreamBfcsd+i*16,&csdTableForHeLTF[NTXindex][i*16],pStreamAfcsd+i*16);
	}
	//free(pStreamBfcsd_expan);
}
#endif
