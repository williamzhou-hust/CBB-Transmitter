#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"
#include "../headers/process.h"
#include <math.h>
#include <stdlib.h>
#include <memory.h>

void ifftShiftandIFFTPreamble(complex32* pstfAfterCSD,complex32* pAfterIFFT,int N_tone){
     //include insert 0 and ifftshift
     complex* pBeforeIFFT=(complex*)malloc(128*sizeof(complex));
     MKSUREENMEM(pBeforeIFFT);
     memset(pBeforeIFFT,0,128*sizeof(complex));
     int i=0;
     for(i=0;i<32;i++){
        pBeforeIFFT[i].real=pstfAfterCSD[32+i].real;
        pBeforeIFFT[i].imag=pstfAfterCSD[32+i].imag;
        pBeforeIFFT[96+i].real=pstfAfterCSD[i].real;
        pBeforeIFFT[96+i].imag=pstfAfterCSD[i].imag;
     }
     //for test
     /*fp=fopen("stfBeforeIFFT.txt","w");
     for(i=0;i<128;i++) fprintf(fp,"%f %f\n",pBeforeIFFT[i].real,pBeforeIFFT[i].imag);
     fclose(fp);*/
     //for test
     //deal with IFFT
     complex* pAfterIFFTtemp=(complex*)malloc(128*sizeof(complex));
     MKSUREENMEM(pAfterIFFTtemp);
     memset(pAfterIFFTtemp,0,128*sizeof(complex));
     IFFT(pBeforeIFFT,pAfterIFFTtemp,128);
     //power normalization, it may put into addCP
     for(i=0;i<128;i++){
        pAfterIFFT[i].real=pAfterIFFTtemp[i].real*(128/sqrt(N_tone*N_TX));//STF:36.9504172281361
        pAfterIFFT[i].imag=pAfterIFFTtemp[i].imag*(128/sqrt(N_tone*N_TX));
     }
     //for test
     /*fp=fopen("stfAfterIFFT.txt","w");
     for(i=0;i<128;i++) fprintf(fp,"%f %f\n",pAfterIFFT[i].real,pAfterIFFT[i].imag);
     fclose(fp);*/
     //for test
     free(pBeforeIFFT);
     free(pAfterIFFTtemp);
}
