#include "../headers/commonStructure.h"
#include "../headers/process.h"
#include "../headers/globalVarINIT.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>

void generateSig(complex32* basicSig, complex32* oneStreamOfSig, int NTXindex){
     int lengthAfterCSD=57;
     complex32* psigAfterCSD=(complex32*)malloc(64*sizeof(complex32));
     MKSUREENMEM(psigAfterCSD);
     memset(psigAfterCSD,0,64*sizeof(complex32));
     //CSD for LTF
     csdForPreamble(basicSig+4, psigAfterCSD+4, NTXindex, lengthAfterCSD);
     //for test
     /*FILE* fp=fopen("sigAfterCSD.txt","w");
     printStreamToFile(psigAfterCSD,64,fp);
     fclose(fp);*/
     //for test
     //IFFTShift and IFFT
     complex32* pAfterIFFT=(complex32*)malloc(128*sizeof(complex32));
     MKSUREENMEM(pAfterIFFT);
     ifftShiftandIFFTPreamble(psigAfterCSD,pAfterIFFT,56);//Sig N_tone=56
     //add cp and window
     complex32* pBeforeAddWin=(complex32*)malloc(163*sizeof(complex32));
     MKSUREENMEM(pBeforeAddWin);
     memset(pBeforeAddWin,0,163*sizeof(complex32));
     addCPforSig(pAfterIFFT,pBeforeAddWin,NTXindex);
     //generate output data
     memcpy(oneStreamOfSig,pBeforeAddWin,160*sizeof(complex32));
     //free malloc data
     free(psigAfterCSD);
     free(pAfterIFFT);
     free(pBeforeAddWin);
}
