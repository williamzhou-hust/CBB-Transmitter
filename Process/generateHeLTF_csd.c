#include "../headers/globalVarINIT.h"
#include "../headers/commonStructure.h"
#include "../headers/integerTypeDef.h"
#include "../headers/process.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

void generateHeLTF_csd(complex32* pHeLTFStart,int NTX_index,int n_ltf){
    complex32* p_matrix=P[N_TX-1];
    //allocate pbeforeCSD
    complex32* pbeforeCSD=(complex32*)malloc(n_ltf*256*sizeof(complex32));
    MKSUREENMEM(pbeforeCSD);
    memset(pbeforeCSD,0,n_ltf*256*sizeof(complex32));
    //add sub pilot
    complex32* p=pbeforeCSD;
    int lenPmatrix=(!(N_TX&(N_TX-1)))?N_TX:(N_TX+1);
    //basicHeLTF pilot_index[8] use B matrix
    int i=0,j=0,k=0;
    for(i=0;i<n_ltf;i++){
        for(j=0;j<256;j++){
            if(j!=pilot_index[k]){
                *p++=multForComplex32(basicHeLTF[j],p_matrix[NTX_index*lenPmatrix+i]);
            }
            else{
                *p++=multForComplex32(basicHeLTF[j],p_matrix[i]);
                ++k;
                if(k==8) k=0;
            }
        }
    }
     //for test
     /*FILE* fp=fopen("HeLTFbeforeCSD.txt","w");
     printStreamToFile(pbeforeCSD,n_ltf*256,fp);
     fclose(fp);*/
     //for test
     for(i=0;i<n_ltf;i++)
         csdForHeLTF(pbeforeCSD+i*256, pHeLTFStart+i*256, NTX_index, 256);
     //free data
     free(pbeforeCSD);
}
