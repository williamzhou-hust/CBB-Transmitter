#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "allHeaders.h"

//#define RUN
#ifdef RUN

int main(int argc, char* argv[]){
    complex32* STF[N_TX];
    complex32* LTF[N_TX];
    complex32* Sig[N_TX];
    //set the information of sig
    unsigned char SigInfo[3];
    setSigInfo(SigInfo,3);
    int i=0;
    for(i=0;i<N_STS;i++){
        STF[i]=(complex32*)malloc(320*sizeof(complex32));
        LTF[i]=(complex32*)malloc(320*sizeof(complex32));
        Sig[i]=(complex32*)malloc(160*sizeof(complex32));
        MKSUREENMEM(STF[i]);
        MKSUREENMEM(LTF[i]);
        MKSUREENMEM(Sig[i])
        memset(STF[i],0,320*sizeof(complex32));
        memset(LTF[i],0,320*sizeof(complex32));
        memset(Sig[i],0,160*sizeof(complex32));
        complex32* pStfStart = STF[i];
        complex32* pLtfStart = LTF[i];
        complex32* pSigStart = Sig[i];
        generatePreamble(pStfStart,pLtfStart,SigInfo,pSigStart,i);
    }
    //save STF data
    FILE* fp=fopen("STF.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(STF[i],320,fp);
    fclose(fp);
    //save LTF data
    fp=fopen("LTF.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(LTF[i],320,fp);
    fclose(fp);
    //save Sig data
    fp=fopen("Sig.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(Sig[i],160,fp);
    fclose(fp);
    //free malloc data
    for(i=0;i<N_TX;i++) free(STF[i]);
    for(i=0;i<N_TX;i++) free(LTF[i]);
    for(i=0;i<N_TX;i++) free(Sig[i]);
    //return
    return 0;
}

void printStream(complex32* pData, int length){
    int n=length;
    while(n--){
        printForCpmlex32(*pData++);
    }
}

void printStreamToFile(complex32* pData, int length, FILE* fp){
    int n=length;
    while(n--){
        fprintf(fp,"%d %d\n",pData->real,pData->imag);
        ++pData;
    }
}

#endif // RUN
