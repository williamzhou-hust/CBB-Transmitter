#include "../headers/commonStructure.h"
#include "../headers/process.h"
#include "../headers/globalVarINIT.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>

void generateSig_csd(complex32* basicSig, complex32* oneStreamOfSig, int NTXindex){
     int lengthAfterCSD=57;
     //complex32* psigAfterCSD=(complex32*)malloc(64*sizeof(complex32));
     //MKSUREENMEM(psigAfterCSD);
     //memset(psigAfterCSD,0,64*sizeof(complex32));
     //CSD for LTF
	 #define AVX2
	 #ifndef AVX2
     csdForPreamble(basicSig+4, oneStreamOfSig+4, NTXindex, lengthAfterCSD);
	 #else
     csdForPreamble(basicSig+4, oneStreamOfSig, NTXindex, lengthAfterCSD);
	 #endif
     //for test
     /*FILE* fp=fopen("sigAfterCSD.txt","w");
     printStreamToFile(psigAfterCSD,64,fp);
     fclose(fp);*/
     //for test
}
