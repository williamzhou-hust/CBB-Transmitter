#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"
#include "../headers/process.h"
#include <memory.h>

void addCPforPreamble(complex32* pAfterIFFT,complex32* pBeforeAddWin,int NTXindex){
     pBeforeAddWin[0].real=pAfterIFFT[62].real;
     pBeforeAddWin[0].imag=pAfterIFFT[62].imag;
     pBeforeAddWin[1].real=pAfterIFFT[63].real;
     pBeforeAddWin[1].imag=pAfterIFFT[63].imag;
     pBeforeAddWin[322].real=pAfterIFFT[0].real;
     pBeforeAddWin[322].imag=pAfterIFFT[0].imag;
     //add cp
     int i=0;
     /*for(i=2;i<66;i++){
        pBeforeAddWin[i].real=pAfterIFFT[62+i].real;
        pBeforeAddWin[i].imag=pAfterIFFT[62+i].imag;
        pBeforeAddWin[i+64].real=pAfterIFFT[i-2].real;
        pBeforeAddWin[i+64].imag=pAfterIFFT[i-2].imag;
        pBeforeAddWin[i+128].real=pAfterIFFT[62+i].real;
        pBeforeAddWin[i+128].imag=pAfterIFFT[62+i].imag;
        pBeforeAddWin[i+192].real=pAfterIFFT[i-2].real;
        pBeforeAddWin[i+192].imag=pAfterIFFT[i-2].imag;
        pBeforeAddWin[i+256].real=pAfterIFFT[62+i].real;
        pBeforeAddWin[i+256].imag=pAfterIFFT[62+i].imag;
     }*/
     memcpy(pBeforeAddWin+2,pAfterIFFT+64,64*sizeof(complex32));
     memcpy(pBeforeAddWin+66,pAfterIFFT,128*sizeof(complex32));
     memcpy(pBeforeAddWin+194,pAfterIFFT,128*sizeof(complex32));
     //add Window
     int WT[3]={1200,4096,6992};
     for(i=0;i<3;i++){
        int32 real=pBeforeAddWin[i].real*WT[i]+regForAddWin[NTXindex][i].real*WT[2-i];
        int32 imag=pBeforeAddWin[i].imag*WT[i]+regForAddWin[NTXindex][i].imag*WT[2-i];
        pBeforeAddWin[i].real=real>>dotscale;
        pBeforeAddWin[i].imag=imag>>dotscale;
        regForAddWin[NTXindex][i].real=pBeforeAddWin[320+i].real;
        regForAddWin[NTXindex][i].imag=pBeforeAddWin[320+i].imag;
     }
}

