#ifndef COMMONSTRUC
#define COMMONSTRUC
#include "../headers/commonStructure.h"
#include "../headers/integerTypeDef.h"
#include "../headers/globalVarINIT.h"

complex multForComplex(complex ra,complex rb){
    complex ans;//{ra.real*rb.real-ra.imag*rb.imag,ra.real*rb.imag+ra.imag*rb.real};
    int32 real=0,imag=0;
    real=ra.real*rb.real-ra.imag*rb.imag;
    imag=ra.real*rb.imag+ra.imag*rb.real;
    ans.real=real>>dotscale;
    ans.imag=imag>>dotscale;
    return ans;
}
complex divForComplex(complex ra,complex rb){
    float coe=(0x1<<dotscale)*1.0/(rb.real*rb.real+rb.imag*rb.imag);
    float tempreal=0,tempimag=0;
    tempreal=ra.real*rb.real+ra.imag*rb.imag;
    tempimag=-ra.real*rb.imag+ra.imag*rb.real;
    complex ans={coe*tempreal,coe*tempimag};
    //ans.real=coe*tempreal;
    //ans.imag=coe*tempimag;
    return ans;
}
complex addForComplex(complex ra,complex rb){
    complex ans={ra.real+rb.real,ra.imag+rb.imag};
    //ans.real=ra.real+rb.real;
    //ans.imag=ra.imag+rb.imag;
    return ans;
}
complex reduceForComplex(complex ra,complex rb){
    complex ans={ra.real-rb.real,ra.imag-rb.imag};
    //ans.real=ra.real-rb.real;
    //ans.imag=ra.imag-rb.imag;
    return ans;
}

//use short integer
complex32 multForComplex32(complex32 ra,complex32 rb){
    complex32 ans;//{ra.real*rb.real-ra.imag*rb.imag,ra.real*rb.imag+ra.imag*rb.real};
    int32 treal=ra.real*rb.real-ra.imag*rb.imag;
    int32 timag=ra.real*rb.imag+ra.imag*rb.real;
    ans.real=treal>>dotscale;
    ans.imag=timag>>dotscale;
    return ans;
}
complex32 divForComplex32(complex32 ra,complex32 rb){
    float coe=(0x1<<dotscale)*1.0/(rb.real*rb.real+rb.imag*rb.imag);//8192.0
    int32 tempreal=0,tempimag=0;
    tempreal=ra.real*rb.real+ra.imag*rb.imag;
    tempimag=-ra.real*rb.imag+ra.imag*rb.real;
    complex32 ans;
    ans.real=coe*tempreal;
    ans.imag=coe*tempimag;
    return ans;
}
complex32 addForComplex32(complex32 ra,complex32 rb){
    complex32 ans={ra.real+rb.real,ra.imag+rb.imag};
    //ans.real=ra.real+rb.real;
    //ans.imag=ra.imag+rb.imag;
    return ans;
}
complex32 reduceForComplex32(complex32 ra,complex32 rb){
    complex32 ans={ra.real-rb.real,ra.imag-rb.imag};
    //ans.real=ra.real-rb.real;
    //ans.imag=ra.imag-rb.imag;
    return ans;
}

//use pointer
void Add_Complex32(complex32* src1,complex32* src2,complex32* dst){
    dst->imag=src1->imag+src2->imag;
    dst->real=src1->real+src2->real;
}

void Sub_Complex32(complex32* src1,complex32* src2,complex32* dst){
    dst->imag=src1->imag-src2->imag;
    dst->real=src1->real-src2->real;
}

void Multy_Complex32(complex32* ra,complex32* rb,complex32* dst){
    //{ra.real*rb.real-ra.imag*rb.imag,ra.real*rb.imag+ra.imag*rb.real};
    int32 treal=ra->real*rb->real-ra->imag*rb->imag;
    int32 timag=ra->real*rb->imag+ra->imag*rb->real;
    dst->real=treal>>dotscale;
    dst->imag=timag>>dotscale;
}

void Divide_Complex32(complex32* ra,complex32* rb,complex32* dst){
    float coe=(0x1<<dotscale)*1.0/(rb->real*rb->real+rb->imag*rb->imag);//8192.0
    int32 tempreal=0,tempimag=0;
    tempreal=ra->real*rb->real+ra->imag*rb->imag;
    tempimag=-ra->real*rb->imag+ra->imag*rb->real;
    dst->real=coe*tempreal;
    dst->imag=coe*tempimag;
}
#endif
