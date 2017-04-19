#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

void generateSTF(complex32* oneStreamOfSTF, int NTXindex){//int lengthofSTF
     int lengthAfterCSD=53;
     complex32* pstfAfterCSD=(complex32*)malloc(64*sizeof(complex32));
     MKSUREENMEM(pstfAfterCSD);
     memset(pstfAfterCSD,0,64*sizeof(complex32));
     //CSD for STF
     csdForPreamble(basicSTF, pstfAfterCSD+6, NTXindex, lengthAfterCSD);
     //for test
     /*FILE* fp=fopen("stfAfterCSD.txt","w");
     printStreamToFile(pstfAfterCSD,64,fp);
     fclose(fp);*/
     //for test
     //IFFTShift and IFFT
     complex32* pAfterIFFT=(complex32*)malloc(128*sizeof(complex32));
     MKSUREENMEM(pAfterIFFT);
     ifftShiftandIFFTPreamble(pstfAfterCSD,pAfterIFFT,12);
     //add cp and window
     complex32* pBeforeAddWin=(complex32*)malloc(323*sizeof(complex32));
     MKSUREENMEM(pBeforeAddWin);
     memset(pBeforeAddWin,0,323*sizeof(complex32));
     addCPforPreamble(pAfterIFFT,pBeforeAddWin,NTXindex);
     //generate output data
     memcpy(oneStreamOfSTF,pBeforeAddWin,320*sizeof(complex32));
     //free malloc data
     free(pstfAfterCSD);
     free(pAfterIFFT);
     free(pBeforeAddWin);
}
