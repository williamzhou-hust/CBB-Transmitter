#include "../headers/process.h"

void bccInterleaverForSig(unsigned char* sigAfterBCC, int length){
     unsigned char w[48]={0};
     int k=0;
     //the first interleaver
     for(k=1;k<49;k++){
        int k1=k-1;
        int i=4*(k1%12)+(k1/12);
        w[i]=sigAfterBCC[k-1];
     }
     //the second interleaver bacause nss==1 three is not
     for(k=1;k<49;k++){
        int k1=k-1;
        //int j=k1+(k1+48-12*k1/48)%1;
        sigAfterBCC[k1]=w[k-1];
     }
}
