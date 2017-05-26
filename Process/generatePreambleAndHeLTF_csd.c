#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "../allHeaders.h"
#include <math.h>


int numberOfHeLTF();
void generatePreambleAndHeLTF_csd()
{
    int n_ltf=0;
    int N_SYM=0;
    //set the information of sig
    unsigned char SigInfo[3];
    setSigInfo(SigInfo,3);
    int i=0;
    for(i=0;i<N_STS;i++){
        STF[i]=(complex32*)malloc(64*sizeof(complex32));
        LTF[i]=(complex32*)malloc(64*sizeof(complex32));
        Sig[i]=(complex32*)malloc(64*sizeof(complex32));
        MKSUREENMEM(STF[i]);
        MKSUREENMEM(LTF[i]);
        MKSUREENMEM(Sig[i]);
        memset(STF[i],0,64*sizeof(complex32));
        memset(LTF[i],0,64*sizeof(complex32));
        memset(Sig[i],0,64*sizeof(complex32));
        complex32* pStfStart = STF[i];
        complex32* pLtfStart = LTF[i];
        complex32* pSigStart = Sig[i];
        generatePreamble_csd(pStfStart,pLtfStart,SigInfo,pSigStart,i);
	    }
    //HE LTF
    //length of HeLTF
    n_ltf=numberOfHeLTF();
    for(i=0;i<N_TX;i++)
    {
        heLTF[i]=(complex32*)malloc(n_ltf*256*sizeof(complex32));
        complex32* pHeLTFStart=heLTF[i];
        generateHeLTF_csd(pHeLTFStart,i,n_ltf);
    }
    
    #define testforinit 
    #ifdef testforinit
    //save STF data
    FILE *fp=fopen("STF_csd.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(STF[i],64,fp);
    fclose(fp);
    //save LTF data
    fp=fopen("LTF_csd.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(LTF[i],64,fp);
    fclose(fp);
    //save Sig data
    fp=fopen("Sig_csd.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(Sig[i],64,fp);
    fclose(fp);
    //save HeLTF data
    fp=fopen("HeLTF_csd.txt", "w");
    for(i=0;i<N_TX;i++) printStreamToFile(heLTF[i],n_ltf*256,fp);
    fclose(fp);
    #endif
    
}


int numberOfData(){
    int N_service = 16;
    int N_tail = 6;
    unsigned char rate_type;
    int N_BPSCS, N_DBPS,N_CBPS, N_ES, N_SYM;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);
    N_SYM = ceil(((double)(8*APEP_LEN + N_service + N_tail*N_ES) / (double)N_DBPS));
    return N_SYM;
}



void printStreamToFile(complex32* pData, int length, FILE* fp){
    int n=length;
    while(n--){
        fprintf(fp,"%d %d\r\n",pData->real,pData->imag);
        ++pData;
    }
}




int numberOfHeLTF()
{
       int ntx=N_TX;
       if(!(ntx&(ntx-1))) return ntx;
       else return ntx+1;
}

