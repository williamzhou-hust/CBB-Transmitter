#include <math.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

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
