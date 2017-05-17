#include <stdio.h>
#include "allHeaders.h"

//#define TEST
#ifdef TEST
int main(int argc,char* argv[]){
    float a=0;
    short b=0;
    int c=0;
    long long d=0;
    printf("Size of float is %d.\n",sizeof(a));
    printf("Size of short is %d.\n",sizeof(b));
    printf("Size of int is %d.\n",sizeof(c));
    printf("Size of long long is %d.\n",sizeof(d));
    return 0;
}
#endif

//#define DEBUGFORCS
#ifdef DEBUGFORCS
int main(int argc,char* argv[]){
    complex32 a={1.1123*8192,1.2215*8192};
    complex32 b={1.9043*8192,0.5672*8192};
    void printForCpmlex32(complex32 rp);
    /*printForCpmlex32(a);
    printForCpmlex32(b);
    printForCpmlex32(multForComplex32(a,b));
    printForCpmlex32(divForComplex32(a,b));
    printForCpmlex32(addForComplex32(a,b));
    printForCpmlex32(reduceForComplex32(a,b));*/
    int c=0;
    printf("c definition is here.\n");
    printf("complex32 is %d large.\n",sizeof(complex32));
    int i=0;
    for (i=0;i<4;i++){
        printf("test for cstableForPreamble %d.\n",csTableForPreamble(i));
    }
    return 0;
}

void printForCpmlex(complex rp){
    printf("real:%1.11f,imag:%1.11f\n",rp.real,rp.imag);
}

void printForCpmlex32(complex32 rp){
    printf("real:%d,imag:%d\n",rp.real,rp.imag);
}
#endif // DEBUGFORCS