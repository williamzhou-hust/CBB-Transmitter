#ifndef HEADCOMMONSTRUC
#define HEADCOMMONSTRUC
#include "integerTypeDef.h"

typedef struct floatcomplex{
       float real;
       float imag;
}complex;

extern complex multForComplex(complex ra,complex rb);
extern complex divForComplex(complex ra,complex rb);
extern complex addForComplex(complex ra,complex rb);
extern complex reduceForComplex(complex ra,complex rb);

//for ifft use
extern void Add_Complex(complex* src1,complex* src2,complex* dst);
extern void Sub_Complex(complex* src1,complex* src2,complex* dst);
extern void Multy_Complex(complex* ra,complex* rb,complex* dst);
extern void Divide_Complex(complex* ra,complex* rb,complex* dst);

typedef struct shortcomplex{
       int16 real;
       int16 imag;
}complex32;

extern complex32 multForComplex32(complex32 ra,complex32 rb);
extern complex32 divForComplex32(complex32 ra,complex32 rb);
extern complex32 addForComplex32(complex32 ra,complex32 rb);
extern complex32 reduceForComplex32(complex32 ra,complex32 rb);

//for data use
extern void Add_Complex32(complex32* src1,complex32* src2,complex32* dst);
extern void Sub_Complex32(complex32* src1,complex32* src2,complex32* dst);
extern void Multy_Complex32(complex32* ra,complex32* rb,complex32* dst);
extern void Divide_Complex32(complex32* ra,complex32* rb,complex32* dst);

#endif // COMMONSTRUC
