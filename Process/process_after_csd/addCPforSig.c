#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"
#include "../headers/process.h"
#include <memory.h>

void addCPforSig(complex32* pAfterIFFT,complex32* pBeforeAddWin,int NTXindex){
     pBeforeAddWin[0].real=pAfterIFFT[94].real;
     pBeforeAddWin[0].imag=pAfterIFFT[94].imag;
     pBeforeAddWin[1].real=pAfterIFFT[95].real;
     pBeforeAddWin[1].imag=pAfterIFFT[95].imag;
     pBeforeAddWin[162].real=pAfterIFFT[0].real;
     pBeforeAddWin[162].imag=pAfterIFFT[0].imag;
     //add cp
     int i=0;
     memcpy(pBeforeAddWin+2,pAfterIFFT+96,32*sizeof(complex32));
     memcpy(pBeforeAddWin+34,pAfterIFFT,128*sizeof(complex32));
     //add Window
     int WT[3]={1200,4096,6992};
     for(i=0;i<3;i++){
        int32 real=pBeforeAddWin[i].real*WT[i]+regForAddWin[NTXindex][i].real*WT[2-i];
        int32 imag=pBeforeAddWin[i].imag*WT[i]+regForAddWin[NTXindex][i].imag*WT[2-i];
        pBeforeAddWin[i].real=real>>dotscale;
        pBeforeAddWin[i].imag=imag>>dotscale;
        regForAddWin[NTXindex][i].real=pBeforeAddWin[160+i].real;
        regForAddWin[NTXindex][i].imag=pBeforeAddWin[160+i].imag;
     }
}


