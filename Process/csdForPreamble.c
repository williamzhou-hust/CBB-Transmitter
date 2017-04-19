#include <math.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

void csdForPreamble(complex32* pStreamBfcsd, complex32* pStreamAfcsd, int NTXindex, int length){
     int n=length;
     float phase=-0.00196349541*csTableForPreamble(NTXindex);//-2PI*det_F(312.5KHz)*Tcs
     while(n){
          *pStreamAfcsd++=multForComplex32(*pStreamBfcsd++,expComlex32(phase,length/2+1-n));//STF&LTF(n=53)(-26:26),Sig(n=57)(-28:28)
          --n;
     }
}

int csTableForPreamble(int Ntxindex){
    int index=(N_TX*(N_TX-1))/2+Ntxindex;
    return CyclicShiftTable[index];
}

complex32 expComlex32(float phase, int i){
    complex32 ans;
    ans.real=cos(i*phase)*(0x1<<dotscale);
    ans.imag=sin(i*phase)*(0x1<<dotscale);
    return ans;
}
