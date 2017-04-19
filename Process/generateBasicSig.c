#include "../headers/commonStructure.h"
#include "../headers/process.h"
#include "../headers/globalVarINIT.h"
#include <stdlib.h>
#include <memory.h>

void generateBasicSig(complex32* basicSig, unsigned char* SigInfo, int length){
     unsigned char* sigAfterBCC=(unsigned char*)malloc(48*sizeof(unsigned char));
     MKSUREENMEM(sigAfterBCC);
     memset(sigAfterBCC,0,48*sizeof(unsigned char));
     //BCC
     ccodedot11_init();
     ccodedot11_encode(3, SigInfo, sigAfterBCC, 0);// mode-0 1/2; mode-1 3/4; mode-2 2/3; mode-5 5/6
     //for test
     /*FILE* fp=fopen("sigAfterBCC.txt","w");
     int i=0;
     for(i=0;i<48;i++) fprintf(fp,"%d\n",sigAfterBCC[i]);
     fclose(fp);*/
     //for test
     //Sig interleaver
     bccInterleaverForSig(sigAfterBCC,48);
     //for test
     /*fp=fopen("sigAfterInterleaver.txt","w");
     for(i=0;i<48;i++) fprintf(fp,"%d\n",sigAfterBCC[i]);
     fclose(fp);*/
     //for test
     //MAP for sig
     complex32* sigAfterMap=(complex32*)malloc(48*sizeof(complex32));
     MKSUREENMEM(sigAfterMap);
     memset(sigAfterMap,0,48*sizeof(complex32));
     MapForSig(sigAfterBCC,sigAfterMap,48);
     //add pilot
     void addPilotForSig(complex32* sigAfterMap,complex32* basicSig);
     addPilotForSig(sigAfterMap,basicSig);
     //free data
     free(sigAfterBCC);
     free(sigAfterMap);
}

void addPilotForSig(complex32* sigAfterMap,complex32* basicSig){
     /*length is 64,four 0 in front
     SIG_26 = [1, 1, SigAfterMapper(1:5), P(1), SigAfterMapper(6:18), P(2), SigAfterMapper(19:24),0,...
        SigAfterMapper(25:30), P(3), SigAfterMapper(31:43), P(4), SigAfterMapper(44:end), 1, 1];//size is 57
     */
     int P[4]={1,1,1,-1};
     basicSig[4].real=1<<dotscale;
     basicSig[5].real=1<<dotscale;
     memcpy(basicSig+6,sigAfterMap,5*sizeof(complex32));
     basicSig[11].real=P[0]<<dotscale;
     memcpy(basicSig+12,sigAfterMap+5,13*sizeof(complex32));
     basicSig[25].real=P[1]<<dotscale;
     memcpy(basicSig+26,sigAfterMap+18,6*sizeof(complex32));
     memcpy(basicSig+33,sigAfterMap+24,6*sizeof(complex32));
     basicSig[39].real=P[2]<<dotscale;
     memcpy(basicSig+40,sigAfterMap+30,13*sizeof(complex32));
     basicSig[53].real=P[3]<<dotscale;
     memcpy(basicSig+54,sigAfterMap+43,5*sizeof(complex32));
     basicSig[59].real=1<<dotscale;
     basicSig[60].real=1<<dotscale;
}
