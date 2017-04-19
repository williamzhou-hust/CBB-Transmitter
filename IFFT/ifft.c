#include "../headers/process.h"
#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265359//3.14159265358979323846
#endif

#define VALUE_MAX 1000

void Add_Complex(complex* src1,complex* src2,complex* dst){
    dst->imag=src1->imag+src2->imag;
    dst->real=src1->real+src2->real;
}

void Sub_Complex(complex* src1,complex* src2,complex* dst){
    dst->imag=src1->imag-src2->imag;
    dst->real=src1->real-src2->real;
}

void Multy_Complex(complex* ra,complex* rb,complex* dst){
    //{ra.real*rb.real-ra.imag*rb.imag,ra.real*rb.imag+ra.imag*rb.real};
    int32 treal=ra->real*rb->real-ra->imag*rb->imag;
    int32 timag=ra->real*rb->imag+ra->imag*rb->real;
    dst->real=treal>>dotscale;
    dst->imag=timag>>dotscale;
}

void Divide_Complex(complex* ra,complex* rb,complex* dst){
    float coe=(0x1<<dotscale)*1.0/(rb->real*rb->real+rb->imag*rb->imag);//8192.0
    float tempreal=0,tempimag=0;
    tempreal=ra->real*rb->real+ra->imag*rb->imag;
    tempimag=-ra->real*rb->imag+ra->imag*rb->real;
    dst->real=coe*tempreal;
    dst->imag=coe*tempimag;
}

void getWN(float n,float size_n,complex* dst){
    float x=2.0*M_PI*n/size_n;
    dst->imag=-sin(x)*(0x1<<dotscale);
    dst->real=cos(x)*(0x1<<dotscale);
}

void DFT(complex* src,complex* dst,int size){
    clock_t start,end;
    start=clock();
    int m=0;
    for(m=0;m<size;m++){
        float real=0.0;
        float imag=0.0;
        int n=0;
        for(n=0;n<size;n++){
            float x=M_PI*2*m*n/size;
            real+=(src[n].real*cos(x)+src[n].imag*sin(x));
            imag+=(-src[n].real*sin(x)+src[n].imag*cos(x));
        }
        if(dst!=NULL){
            dst[m].real=real;
            dst[m].imag=imag;
        }
        /*if(imag>=0.0)
            printf("%lf+%lfj\n",real,imag);
        else
            printf("%lf%lfj\n",real,imag);*/
    }
    end=clock();
    printf("IDFT use time :%lfs for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size);
}

void IDFT(complex* src,complex* dst,int size){
    clock_t start,end;
    start=clock();
    int m=0;
    for(m=0;m<size;m++){
        float real=0.0;
        float imag=0.0;
        int n=0;
        for(n=0;n<size;n++){
            float x=M_PI*2*m*n/size;
            real+=(src[n].real*cos(x)-src[n].imag*sin(x));
            imag+=(src[n].real*sin(x)+src[n].imag*cos(x));
        }
        real/=size;
        imag/=size;
        if(dst!=NULL){
            dst[m].real=real;
            dst[m].imag=imag;
        }
        /*if(imag>=0.0)
            printf("%lf+%lfj\n",real,imag);
        else
            printf("%lf%lfj\n",real,imag);*/
    }
    end=clock();
    printf("IDFT use time :%lfs for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size);//(double) CLOCKS_PER_SEC
}

int FFT_remap(complex* src,int size_n){

    if(size_n==1)
        return 0;
    complex* temp=(complex*)malloc(sizeof(complex)*size_n);
    int i=0;
    for(i=0;i<size_n;i++)
        if(i%2==0)
            temp[i/2]=src[i];
        else
            temp[(size_n+i)/2]=src[i];
    for(i=0;i<size_n;i++)
        src[i]=temp[i];
    free(temp);
    FFT_remap(src, size_n/2);
    FFT_remap(src+size_n/2, size_n/2);
    return 1;
}

void FFT(complex* src,complex* dst,int size_n){

    FFT_remap(src, size_n);
   // for(int i=0;i<size_n;i++)
    //    printf("%lf\n",src[i]);
    clock_t start,end;
    start=clock();
    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    complex* src_com=(complex*)malloc(sizeof(complex)*size_n);
    if(src_com==NULL)
        exit(0);
    int i=0;
    for(i=0;i<size_n;i++){
        src_com[i].real=src[i].real;
        src_com[i].imag=src[i].imag;
    }
    for(i=0;i<k;i++){
        z=0;
        int j=0;
        for(j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src_com[j],&wn,&src_com[j]);
                z+=1<<(k-i-1);
                complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imag=src_com[neighbour].imag;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
    }
   /* for(int i=0;i<size_n;i++)
        if(src_com[i].imagin>=0.0){
            printf("%lf+%lfj\n",src_com[i].real,src_com[i].imagin);
        }
        else
            printf("%lf%lfj\n",src_com[i].real,src_com[i].imagin);*/
    for(i=0;i<size_n;i++){
        dst[i].imag=src_com[i].imag;
        dst[i].real=src_com[i].real;
    }
    end=clock();
    printf("FFT use time :%lfs for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size_n);
}

void IFFT(complex* src,complex* dst,int size_n){

    FFT_remap(src, size_n);
   // for(int i=0;i<size_n;i++)
    //    printf("%lf\n",src[i]);
    clock_t start,end;
    start=clock();
    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    complex* src_com=(complex*)malloc(sizeof(complex)*size_n);
    if(src_com==NULL)
        exit(0);
    int i=0;
    for(i=0;i<size_n;i++){
        src_com[i].real=src[i].real;
        src_com[i].imag=src[i].imag;
    }
    for(i=0;i<k;i++){
        z=0;
        int j=0;
        for(j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                complex wn;
                getWN(-z, size_n, &wn);
                Multy_Complex(&src_com[j],&wn,&src_com[j]);
                z+=1<<(k-i-1);
                complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imag=src_com[neighbour].imag;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
    }
   /* for(int i=0;i<size_n;i++)
        if(src_com[i].imagin>=0.0){
            printf("%lf+%lfj\n",src_com[i].real,src_com[i].imagin);
        }
        else
            printf("%lf%lfj\n",src_com[i].real,src_com[i].imagin);*/
    for(i=0;i<size_n;i++){
        dst[i].imag=src_com[i].imag/size_n;
        dst[i].real=src_com[i].real/size_n;
    }
    end=clock();
    printf("IFFT use time :%lfs for Datasize of:%d\n",(double)(end-start)/CLOCKS_PER_SEC,size_n);//(double) CLOCKS_PER_SEC
}

//test for ifft
//#define IFFTDEBUG
#ifdef IFFTDEBUG
int main(int argc, const char * argv[]){

    FILE* fp=fopen("ifftdata_512.txt","r");
    if(fp==NULL) return 1;
    const int N=512;
    complex *ifftdata=(complex*)malloc(N*sizeof(complex));
    if(ifftdata==NULL){printf("Not enough memory!\n");return 1;};
    int i=0;
    for(i=0;i<N;i++){
        fscanf(fp,"%f %f",&ifftdata[i].real,&ifftdata[i].imag);
    }
    fclose(fp);

    //IDFT not change sourse data
    complex *idftdest=(complex*)malloc(N*sizeof(complex));
    if(idftdest==NULL){printf("Not enough memory!\n");return 1;};
    IDFT(ifftdata,idftdest,N);
    fp=fopen("idft_dest_512.txt","w");
    if(fp==NULL) return 1;
    for(i=0;i<N;i++){
        fprintf(fp,"%f %f\n",idftdest[i].real,idftdest[i].imag);
    }
    fclose(fp);

    //IFFT it will change the sourse data
    complex *ifftdest=(complex*)malloc(N*sizeof(complex));
    if(ifftdest==NULL){printf("Not enough memory!\n");return 1;};
    IFFT(ifftdata,ifftdest,N);
    fp=fopen("ifft_dest_512.txt","w");
    if(fp==NULL) return 1;
    for(i=0;i<N;i++){
        fprintf(fp,"%f %f\n",ifftdest[i].real,ifftdest[i].imag);
    }
    fclose(fp);

    //free malloc data
    free(ifftdata);
    free(ifftdest);
    free(idftdest);
    return 0;
}
#endif//IFFTDEBUG
