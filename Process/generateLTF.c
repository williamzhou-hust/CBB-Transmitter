#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

void generateLTF(complex32* oneStreamOfLTF, int NTXindex){
     int lengthAfterCSD=53;
     complex32* pltfAfterCSD=(complex32*)malloc(64*sizeof(complex32));
     MKSUREENMEM(pltfAfterCSD);
     memset(pltfAfterCSD,0,64*sizeof(complex32));
     //CSD for LTF
     csdForPreamble(basicLTF, pltfAfterCSD+6, NTXindex, lengthAfterCSD);
     //for test
     /*FILE* fp=fopen("ltfAfterCSD.txt","w");
     printStreamToFile(pltfAfterCSD,64,fp);
     fclose(fp);*/
     //for test
     //IFFTShift and IFFT
     complex32* pAfterIFFT=(complex32*)malloc(128*sizeof(complex32));
     MKSUREENMEM(pAfterIFFT);
     ifftShiftandIFFTPreamble(pltfAfterCSD,pAfterIFFT,52);//LTF N_tone=52
     //add cp and window
     complex32* pBeforeAddWin=(complex32*)malloc(323*sizeof(complex32));
     MKSUREENMEM(pBeforeAddWin);
     memset(pBeforeAddWin,0,323*sizeof(complex32));
     addCPforPreamble(pAfterIFFT,pBeforeAddWin,NTXindex);
     //generate output data
     memcpy(oneStreamOfLTF,pBeforeAddWin,320*sizeof(complex32));
     //free malloc data
     free(pltfAfterCSD);
     free(pAfterIFFT);
     free(pBeforeAddWin);
}

