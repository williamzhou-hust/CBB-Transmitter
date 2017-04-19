#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

void generateSTF_csd(complex32* oneStreamOfSTF, int NTXindex){//int lengthofSTF
     int lengthAfterCSD=53;
     //complex32* pstfAfterCSD=(complex32*)malloc(64*sizeof(complex32));
     //MKSUREENMEM(pstfAfterCSD);
     //memset(pstfAfterCSD,0,64*sizeof(complex32));
     //CSD for STF
     csdForPreamble(basicSTF, oneStreamOfSTF+6, NTXindex, lengthAfterCSD);
     //for test
     /*FILE* fp=fopen("stfAfterCSD.txt","w");
     printStreamToFile(pstfAfterCSD,64,fp);
     fclose(fp);*/
     //for test
}
