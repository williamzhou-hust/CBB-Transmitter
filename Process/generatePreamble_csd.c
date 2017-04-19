#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"
#include <stdlib.h>
#include <memory.h>

void generatePreamble_csd(complex32* oneStreamOfSTF,
                      complex32* oneStreamOfLTF,
                      unsigned char* SigInfo,
                      complex32* oneStreamOfSig, int NTXindex){
     //generate STF lengthofSTF=320;
     generateSTF_csd(oneStreamOfSTF,NTXindex);
     //generate LTF lengthofLTF=320;
     generateLTF_csd(oneStreamOfLTF,NTXindex);
     //generate basicSig data
     complex32* basicSig=(complex32*)malloc(64*sizeof(complex32));
     MKSUREENMEM(basicSig);
     memset(basicSig,0,64*sizeof(complex32));
     generateBasicSig(basicSig,SigInfo,64);
     //for test
     /*FILE* fp=fopen("basicSig.txt","w");
     int i=0;
     for(i=0;i<64;i++) fprintf(fp,"%d %d\n",basicSig[i].real,basicSig[i].imag);
     fclose(fp);*/
     //for test
     //generate Sig
     generateSig_csd(basicSig,oneStreamOfSig,NTXindex);
     //free data
     free(basicSig);
}



