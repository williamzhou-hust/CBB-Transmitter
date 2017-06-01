// XLZL.cpp : Defines the entry point for the console application.
//
#include <immintrin.h>//main header
#include <emmintrin.h>//__m128i
#include <time.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/process.h"
#ifndef DEBUGAVX2
#include "../headers/commonStructure.h"
#endif
#define _MM_ALIGN32 __attribute__ ((aligned (32)))
#define Matrix_H 4

#ifdef DEBUGAVX2
typedef short int16;
typedef int int32;
struct shortcomplex{
	int16 real;
	int16 imag;
};
typedef struct shortcomplex complex32;

int dotscale = 13;
complex32 multForComplex32(complex32 ra, complex32 rb){
	complex32 ans;//{ra.real*rb.real-ra.imag*rb.imag,ra.real*rb.imag+ra.imag*rb.real};
	int32 treal = ra.real*rb.real - ra.imag*rb.imag;
	int32 timag = ra.real*rb.imag + ra.imag*rb.real;
	ans.real = treal >> dotscale;
	ans.imag = timag >> dotscale;
	return ans;
}
complex32 divForComplex32(complex32 ra, complex32 rb){
	float coe = (0x1 << dotscale)*1.0 / (rb.real*rb.real + rb.imag*rb.imag);//8192.0
	int32 tempreal = 0, tempimag = 0;
	tempreal = ra.real*rb.real + ra.imag*rb.imag;
	tempimag = -ra.real*rb.imag + ra.imag*rb.real;
	complex32 ans;
	ans.real = coe*tempreal;
	ans.imag = coe*tempimag;
	return ans;
}
complex32 addForComplex32(complex32 ra, complex32 rb){
	complex32 ans = { ra.real + rb.real, ra.imag + rb.imag };
	//ans.real=ra.real+rb.real;
	//ans.imag=ra.imag+rb.imag;
	return ans;
}
complex32 reduceForComplex32(complex32 ra, complex32 rb){
	complex32 ans = { ra.real - rb.real, ra.imag - rb.imag };
	//ans.real=ra.real-rb.real;
	//ans.imag=ra.imag-rb.imag;
	return ans;
}
#endif

//Matrix_Mult for 4x4
void multForMatrix(complex32 (*h)[Matrix_H],complex32* x,complex32* dest){
	int i=0,j=0;
	for(j=0;j<Matrix_H;j++){
		complex32 temp={0,0};
		for(i=0;i<Matrix_H;i++){
			temp=addForComplex32(temp,multForComplex32(h[i][j],x[i]));
		}
		dest[j].real=temp.real;
		dest[j].imag=temp.imag;
	}
}

//Matrix_Mult for 4x4 use avx2
//make sure dest's data equal 0
void Mult_Matrix_AVX2_4(complex32 (*h)[4],complex32* x,complex32* dest){
	complex32 a[16]={h[0][0],h[1][0],h[2][0],h[3][0],
					 h[0][1],h[1][1],h[2][1],h[3][1],
					 h[0][2],h[1][2],h[2][2],h[3][2],
					 h[0][3],h[1][3],h[2][3],h[3][3]};
	complex32 b[16]={x[0],x[1],x[2],x[3],
					 x[0],x[1],x[2],x[3],
					 x[0],x[1],x[2],x[3],
					 x[0],x[1],x[2],x[3]};
	complex32 c[16]={{0,0}};
	Mult_complex32Vector_2(a,b,c);
	complex32 temp[4] _MM_ALIGN32 = {{0,0},{0,0},{0,0},{0,0}};
	int16 c1[8] _MM_ALIGN32 = {c[0].real,c[0].imag,c[4].real,c[4].imag,c[8].real,c[8].imag,c[12].real,c[12].imag};
	int16 c2[8] _MM_ALIGN32 = {c[1].real,c[1].imag,c[5].real,c[5].imag,c[9].real,c[9].imag,c[13].real,c[13].imag}; 
	int16 c3[8] _MM_ALIGN32 = {c[2].real,c[2].imag,c[6].real,c[6].imag,c[10].real,c[10].imag,c[14].real,c[14].imag}; 
	int16 c4[8] _MM_ALIGN32 = {c[3].real,c[3].imag,c[7].real,c[7].imag,c[11].real,c[11].imag,c[15].real,c[15].imag}; 
	/*int i=0,j=0;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			temp[i]=addForComplex32(temp[i],c[i*4+j]);
		}
	dest[i].real=temp[i].real;
	dest[i].imag=temp[i].imag;
	}*/
	//__m128i _mm_load_si128 (__m128i const* mem_addr)
	//__m128i _mm_add_epi16 (__m128i a, __m128i b)
	__m128i temp1,temp2,temp3,temp4;
	temp1=_mm_load_si128((__m128i*)c1);
	temp2=_mm_load_si128((__m128i*)c2);
	temp3=_mm_load_si128((__m128i*)c3);
	temp4=_mm_load_si128((__m128i*)c4);
	temp1=_mm_add_epi16(temp1,temp2);
	temp1=_mm_add_epi16(temp1,temp3);
	temp1=_mm_add_epi16(temp1,temp4);
	//_mm_store_si128 (__m128i* mem_addr, __m128i a)
	_mm_store_si128((__m128i*)temp,temp1);
	memcpy(dest,&temp,16);
	return;
}

//Matrix_Mult for 8x8 use avx2
//make sure dest's data equal 0
void Matrix_Mult_AVX2_8(complex32 (*h)[8],complex32* x,complex32* dest){
	complex32 a[4][16];
	complex32 b[4][16];
	complex32 c[4][16];

	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<16;j++){
			a[i][j] = h[j&7][i*2+(j>>3)];
			b[i][j] = x[j&7];
			//c[i][j].real = 0;
			//c[i][j].imag = 0;
		}
	}
	for(i=0;i<4;i++)
		Mult_complex32Vector_2(a[i],b[i],c[i]);

	int16 c1[16] _MM_ALIGN32 = {c[0][0].real,c[0][0].imag,c[0][8].real,c[0][8].imag,c[1][0].real,c[1][0].imag,c[1][8].real,c[1][8].imag,c[2][0].real,c[2][0].imag,c[2][8].real,c[2][8].imag,c[3][0].real,c[3][0].imag,c[3][8].real,c[3][8].imag};
	int16 c2[16] _MM_ALIGN32 = {c[0][1].real,c[0][1].imag,c[0][9].real,c[0][9].imag,c[1][1].real,c[1][1].imag,c[1][9].real,c[1][9].imag,c[2][1].real,c[2][1].imag,c[2][9].real,c[2][9].imag,c[3][1].real,c[3][1].imag,c[3][9].real,c[3][9].imag};
	int16 c3[16] _MM_ALIGN32 = {c[0][2].real,c[0][2].imag,c[0][10].real,c[0][10].imag,c[1][2].real,c[1][2].imag,c[1][10].real,c[1][10].imag,c[2][2].real,c[2][2].imag,c[2][10].real,c[2][10].imag,c[3][2].real,c[3][2].imag,c[3][10].real,c[3][10].imag};
	int16 c4[16] _MM_ALIGN32 = {c[0][3].real,c[0][3].imag,c[0][11].real,c[0][11].imag,c[1][3].real,c[1][3].imag,c[1][11].real,c[1][11].imag,c[2][3].real,c[2][3].imag,c[2][11].real,c[2][11].imag,c[3][3].real,c[3][3].imag,c[3][11].real,c[3][11].imag};
	int16 c5[16] _MM_ALIGN32 = {c[0][4].real,c[0][4].imag,c[0][12].real,c[0][12].imag,c[1][4].real,c[1][4].imag,c[1][12].real,c[1][12].imag,c[2][4].real,c[2][4].imag,c[2][12].real,c[2][12].imag,c[3][4].real,c[3][4].imag,c[3][12].real,c[3][12].imag};
	int16 c6[16] _MM_ALIGN32 = {c[0][5].real,c[0][5].imag,c[0][13].real,c[0][13].imag,c[1][5].real,c[1][5].imag,c[1][13].real,c[1][13].imag,c[2][5].real,c[2][5].imag,c[2][13].real,c[2][13].imag,c[3][5].real,c[3][5].imag,c[3][13].real,c[3][13].imag};
	int16 c7[16] _MM_ALIGN32 = {c[0][6].real,c[0][6].imag,c[0][14].real,c[0][14].imag,c[1][6].real,c[1][6].imag,c[1][14].real,c[1][14].imag,c[2][6].real,c[2][6].imag,c[2][14].real,c[2][14].imag,c[3][6].real,c[3][6].imag,c[3][14].real,c[3][14].imag};
	int16 c8[16] _MM_ALIGN32 = {c[0][7].real,c[0][7].imag,c[0][15].real,c[0][15].imag,c[1][7].real,c[1][7].imag,c[1][15].real,c[1][15].imag,c[2][7].real,c[2][7].imag,c[2][15].real,c[2][15].imag,c[3][7].real,c[3][7].imag,c[3][15].real,c[3][15].imag};
	
	//__m256i _mm256_add_epi16 (__m256i a, __m256i b)
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	complex32 temp[8] _MM_ALIGN32 = {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
	__m256i temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	temp1 = _mm256_load_si256((__m256i*)c1);
	temp2 = _mm256_load_si256((__m256i*)c2);
	temp3 = _mm256_load_si256((__m256i*)c3);
	temp4 = _mm256_load_si256((__m256i*)c4);
	temp5 = _mm256_load_si256((__m256i*)c5);
	temp6 = _mm256_load_si256((__m256i*)c6);
	temp7 = _mm256_load_si256((__m256i*)c7);
	temp8 = _mm256_load_si256((__m256i*)c8);
	
	temp1 = _mm256_add_epi16(temp1,temp2);
	temp1 = _mm256_add_epi16(temp1,temp3);
	temp1 = _mm256_add_epi16(temp1,temp4);
	temp1 = _mm256_add_epi16(temp1,temp5);
	temp1 = _mm256_add_epi16(temp1,temp6);
	temp1 = _mm256_add_epi16(temp1,temp7);
	temp1 = _mm256_add_epi16(temp1,temp8);
	//_mm256_store_si256(__m256i * mem_addr, __m256i a)

	_mm256_store_si256((__m256i *)temp,temp1);
	memcpy(dest,&temp,32);
}


//#pragma pack(4)
void Mult_complex32Vector(complex32* a, complex32* b, complex32* dest /*int lengthOfVector = 8*/){
    float a_real_v[8] _MM_ALIGN32 = { a[0].real, a[1].real, a[2].real, a[3].real, a[4].real, a[5].real, a[6].real, a[7].real };
    float a_imag_v[8] _MM_ALIGN32 = { a[0].imag, a[1].imag, a[2].imag, a[3].imag, a[4].imag, a[5].imag, a[6].imag, a[7].imag };
    float b_real_v[8] _MM_ALIGN32 = { b[0].real, b[1].real, b[2].real, b[3].real, b[4].real, b[5].real, b[6].real, b[7].real };
    float b_imag_v[8] _MM_ALIGN32 = { b[0].imag, b[1].imag, b[2].imag, b[3].imag, b[4].imag, b[5].imag, b[6].imag, b[7].imag };
	__m256 a_real;
	__m256 a_imag;
	__m256 b_real;
	__m256 b_imag;
	a_real = _mm256_load_ps(a_real_v);
	a_imag = _mm256_load_ps(a_imag_v);
	b_real = _mm256_load_ps(b_real_v);
	b_imag = _mm256_load_ps(b_imag_v);
	__m256 a_b_real;
	__m256 a_b_imag;
	__m256 a_real_b_imag;
	__m256 a_imag_b_real;
	a_b_real = _mm256_mul_ps(a_real,b_real);
	a_b_imag = _mm256_mul_ps(a_imag,b_imag);
	a_real_b_imag = _mm256_mul_ps(a_real,b_imag);
	a_imag_b_real = _mm256_mul_ps(a_imag,b_real);
	//__m256 _mm256_sub_ps (__m256 a, __m256 b)
	//__m256 _mm256_add_ps (__m256 a, __m256 b)
	__m256 dest_real;
	__m256 dest_imag;
	dest_real = _mm256_sub_ps(a_b_real, a_b_imag);
	dest_imag = _mm256_add_ps(a_real_b_imag, a_imag_b_real);
    float dest_real_v[8] _MM_ALIGN32;
    float dest_imag_v[8] _MM_ALIGN32;
	_mm256_store_ps(dest_real_v, dest_real);
	_mm256_store_ps(dest_imag_v, dest_imag);
	int i = 0;
	for (i = 0; i < 8; i++){
		dest[i].real = (int)dest_real_v[i] >> 13;//m256_f32
		dest[i].imag = (int)dest_imag_v[i] >> 13;
	}
}

/*use __m256i*/
void Mult_complex32Vector_2(complex32* a, complex32* b, complex32* dest /*int lengthOfVector = 16*/){
    int16 a_real_v[16] _MM_ALIGN32 = { a[0].real, a[1].real, a[2].real, a[3].real, a[4].real, a[5].real, a[6].real, a[7].real,
		a[8].real, a[9].real, a[10].real, a[11].real, a[12].real, a[13].real, a[14].real, a[15].real };
    int16 a_imag_v[16] _MM_ALIGN32 = { a[0].imag, a[1].imag, a[2].imag, a[3].imag, a[4].imag, a[5].imag, a[6].imag, a[7].imag,
		a[8].imag, a[9].imag, a[10].imag, a[11].imag, a[12].imag, a[13].imag, a[14].imag, a[15].imag };
    int16 b_real_v[16] _MM_ALIGN32 = { b[0].real, b[1].real, b[2].real, b[3].real, b[4].real, b[5].real, b[6].real, b[7].real,
		b[8].real, b[9].real, b[10].real, b[11].real, b[12].real, b[13].real, b[14].real, b[15].real };
    int16 b_imag_v[16] _MM_ALIGN32 = { b[0].imag, b[1].imag, b[2].imag, b[3].imag, b[4].imag, b[5].imag, b[6].imag, b[7].imag,
		b[8].imag, b[9].imag, b[10].imag, b[11].imag, b[12].imag, b[13].imag, b[14].imag, b[15].imag };
	__m256i a_real;
	__m256i a_imag;
	__m256i b_real;
	__m256i b_imag;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	a_real = _mm256_load_si256((__m256i*)a_real_v);
	a_imag = _mm256_load_si256((__m256i*)a_imag_v);
	b_real = _mm256_load_si256((__m256i*)b_real_v);
	b_imag = _mm256_load_si256((__m256i*)b_imag_v);
	__m256i a_b_real;
	__m256i a_b_imag;
	__m256i a_real_b_imag;
	__m256i a_imag_b_real;
	//__m256i _mm256_mulhi_epi16 (__m256i a, __m256i b) choose the high 16bit
	//__m256i _mm256_mulhrs_epi16 (__m256i a, __m256i b) >>14 and choose the low 16bit
	a_b_real = _mm256_mulhrs_epi16(a_real, b_real);
	a_b_imag = _mm256_mulhrs_epi16(a_imag, b_imag);
	a_real_b_imag = _mm256_mulhrs_epi16(a_real, b_imag);
	a_imag_b_real = _mm256_mulhrs_epi16(a_imag, b_real);
	//__m256i _mm256_sub_epi16 (__m256i a, __m256i b)
	//__m256i _mm256_add_epi16 (__m256i a, __m256i b)
	__m256i dest_real;
	__m256i dest_imag;
	dest_real = _mm256_sub_epi16(a_b_real, a_b_imag);
	dest_imag = _mm256_add_epi16(a_real_b_imag, a_imag_b_real);
    int16 dest_real_v[16] _MM_ALIGN32;
    int16 dest_imag_v[16] _MM_ALIGN32;
	//_mm256_store_si256
	_mm256_store_si256((__m256i*)dest_real_v,dest_real);
	_mm256_store_si256((__m256i*)dest_imag_v,dest_imag);
	int i = 0;
	for (i = 0; i < 16; i++){
		dest[i].real = dest_real_v[i]<<2;//m256i_i16
		dest[i].imag = dest_imag_v[i]<<2;
	}
}

void Add_complex32Vector_2(complex32* a, complex32* b, complex32* dest /*int lengthOfVector = 16*/){
	int16 a_real_v[16] _MM_ALIGN32 = { a[0].real, a[1].real, a[2].real, a[3].real, a[4].real, a[5].real, a[6].real, a[7].real,
		a[8].real, a[9].real, a[10].real, a[11].real, a[12].real, a[13].real, a[14].real, a[15].real };
	int16 a_imag_v[16] _MM_ALIGN32 = { a[0].imag, a[1].imag, a[2].imag, a[3].imag, a[4].imag, a[5].imag, a[6].imag, a[7].imag,
		a[8].imag, a[9].imag, a[10].imag, a[11].imag, a[12].imag, a[13].imag, a[14].imag, a[15].imag };
	int16 b_real_v[16] _MM_ALIGN32 = { b[0].real, b[1].real, b[2].real, b[3].real, b[4].real, b[5].real, b[6].real, b[7].real,
		b[8].real, b[9].real, b[10].real, b[11].real, b[12].real, b[13].real, b[14].real, b[15].real };
	int16 b_imag_v[16] _MM_ALIGN32 = { b[0].imag, b[1].imag, b[2].imag, b[3].imag, b[4].imag, b[5].imag, b[6].imag, b[7].imag,
		b[8].imag, b[9].imag, b[10].imag, b[11].imag, b[12].imag, b[13].imag, b[14].imag, b[15].imag };
	__m256i a_real;
	__m256i a_imag;
	__m256i b_real;
	__m256i b_imag;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	a_real = _mm256_load_si256((__m256i*)a_real_v);
	a_imag = _mm256_load_si256((__m256i*)a_imag_v);
	b_real = _mm256_load_si256((__m256i*)b_real_v);
	b_imag = _mm256_load_si256((__m256i*)b_imag_v);
	//__m256i _mm256_sub_epi16 (__m256i a, __m256i b)
	//__m256i _mm256_add_epi16 (__m256i a, __m256i b)
	__m256i dest_real;
	__m256i dest_imag;
	dest_real = _mm256_add_epi16(a_real, b_real);
	dest_imag = _mm256_add_epi16(a_imag, b_imag);
	int16 dest_real_v[16] _MM_ALIGN32;
    int16 dest_imag_v[16] _MM_ALIGN32;
	//_mm256_store_si256
	_mm256_store_si256((__m256i*)dest_real_v,dest_real);
	_mm256_store_si256((__m256i*)dest_imag_v,dest_imag);
	int i = 0;
	for (i = 0; i < 16; i++){
		dest[i].real = dest_real_v[i];
		dest[i].imag = dest_imag_v[i];
	}
}

void Sub_complex32Vector_2(complex32* a, complex32* b, complex32* dest /*int lengthOfVector = 16*/){
	int16 a_real_v[16] _MM_ALIGN32 = { a[0].real, a[1].real, a[2].real, a[3].real, a[4].real, a[5].real, a[6].real, a[7].real,
		a[8].real, a[9].real, a[10].real, a[11].real, a[12].real, a[13].real, a[14].real, a[15].real };
	int16 a_imag_v[16] _MM_ALIGN32 = { a[0].imag, a[1].imag, a[2].imag, a[3].imag, a[4].imag, a[5].imag, a[6].imag, a[7].imag,
		a[8].imag, a[9].imag, a[10].imag, a[11].imag, a[12].imag, a[13].imag, a[14].imag, a[15].imag };
	int16 b_real_v[16] _MM_ALIGN32 = { b[0].real, b[1].real, b[2].real, b[3].real, b[4].real, b[5].real, b[6].real, b[7].real,
		b[8].real, b[9].real, b[10].real, b[11].real, b[12].real, b[13].real, b[14].real, b[15].real };
	int16 b_imag_v[16] _MM_ALIGN32 = { b[0].imag, b[1].imag, b[2].imag, b[3].imag, b[4].imag, b[5].imag, b[6].imag, b[7].imag,
		b[8].imag, b[9].imag, b[10].imag, b[11].imag, b[12].imag, b[13].imag, b[14].imag, b[15].imag };
	__m256i a_real;
	__m256i a_imag;
	__m256i b_real;
	__m256i b_imag;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	a_real = _mm256_load_si256((__m256i*)a_real_v);
	a_imag = _mm256_load_si256((__m256i*)a_imag_v);
	b_real = _mm256_load_si256((__m256i*)b_real_v);
	b_imag = _mm256_load_si256((__m256i*)b_imag_v);
	//__m256i _mm256_sub_epi16 (__m256i a, __m256i b)
	//__m256i _mm256_add_epi16 (__m256i a, __m256i b)
	__m256i dest_real;
	__m256i dest_imag;
	dest_real = _mm256_sub_epi16(a_real, b_real);
	dest_imag = _mm256_sub_epi16(a_imag, b_imag);
	int16 dest_real_v[16] _MM_ALIGN32;
    int16 dest_imag_v[16] _MM_ALIGN32;
	//_mm256_store_si256
	_mm256_store_si256((__m256i*)dest_real_v,dest_real);
	_mm256_store_si256((__m256i*)dest_imag_v,dest_imag);
	int i = 0;
	for (i = 0; i < 16; i++){
		dest[i].real = dest_real_v[i];
		dest[i].imag = dest_imag_v[i];
	}
}

//__m256i _mm256_xor_si256 (__m256i a, __m256i b)
//__m256i _mm256_or_si256 (__m256i a, __m256i b)
//__m256i _mm256_and_si256 (__m256i a, __m256i b)

void Xor_Vector_2(unsigned char* a, unsigned char* b, unsigned char* dest /*int lengthOfVector = 32*/){
	unsigned char a_v[32] _MM_ALIGN32 = { a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
		a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
		a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31] };
	unsigned char b_v[32] _MM_ALIGN32 = { b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7],
		b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16], b[17], b[18], b[19], b[20], b[21], b[22], b[23],
		b[24], b[25], b[26], b[27], b[28], b[29], b[30], b[31] };
	__m256i a_i;
	__m256i b_i;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	a_i = _mm256_load_si256((__m256i*)a_v);
	b_i = _mm256_load_si256((__m256i*)b_v);
	//__m256i _mm256_xor_si256 (__m256i a, __m256i b)
	__m256i dest_i;
	dest_i = _mm256_xor_si256(a_i, b_i);
	//_mm256_store_si256
	_mm256_store_si256((__m256i*)dest,dest_i);
}

void Or_Vector_2(unsigned char* a, unsigned char* b, unsigned char* dest /*int lengthOfVector = 32*/){
	unsigned char a_v[32] _MM_ALIGN32 = { a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
		a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
		a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31] };
	unsigned char b_v[32] _MM_ALIGN32 = { b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7],
		b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16], b[17], b[18], b[19], b[20], b[21], b[22], b[23],
		b[24], b[25], b[26], b[27], b[28], b[29], b[30], b[31] };
	__m256i a_i;
	__m256i b_i;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	a_i = _mm256_load_si256((__m256i*)a_v);
	b_i = _mm256_load_si256((__m256i*)b_v);
	//__m256i _mm256_or_si256 (__m256i a, __m256i b)
	__m256i dest_i;
	dest_i = _mm256_or_si256(a_i, b_i);
	//_mm256_store_si256
	_mm256_store_si256((__m256i*)dest,dest_i);
}

void And_Vector_2(unsigned char* a, unsigned char* b, unsigned char* dest /*int lengthOfVector = 32*/){
	unsigned char a_v[32] _MM_ALIGN32 = { a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
		a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15], a[16], a[17], a[18], a[19], a[20], a[21], a[22], a[23],
		a[24], a[25], a[26], a[27], a[28], a[29], a[30], a[31] };
	unsigned char b_v[32] _MM_ALIGN32 = { b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7],
		b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16], b[17], b[18], b[19], b[20], b[21], b[22], b[23],
		b[24], b[25], b[26], b[27], b[28], b[29], b[30], b[31] };
	__m256i a_i;
	__m256i b_i;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	a_i = _mm256_load_si256((__m256i*)a_v);
	b_i = _mm256_load_si256((__m256i*)b_v);
	//__m256i _mm256_and_si256 (__m256i a, __m256i b)
	__m256i dest_i;
	dest_i = _mm256_and_si256(a_i, b_i);
	//_mm256_store_si256
	_mm256_store_si256((__m256i*)dest,dest_i);
}

/*use m512i will be aligned 64*/
/*void Mult_complex32Vector_3(complex32* a, complex32* b, complex32* dest){ //int lengthOfVector = 32
    int16 a_real_v[16] _MM_ALIGN32 = { a[0].real, a[1].real, a[2].real, a[3].real, a[4].real, a[5].real, a[6].real, a[7].real,
		a[8].real, a[9].real, a[10].real, a[11].real, a[12].real, a[13].real, a[14].real, a[15].real, 
		a[16].real, a[17].real, a[18].real, a[19].real, a[20].real, a[21].real, a[22].real, a[23].real,
		a[24].real, a[25].real, a[26].real, a[27].real, a[28].real, a[29].real, a[30].real, a[31].real };
    int16 a_imag_v[16] _MM_ALIGN32 = { a[0].imag, a[1].imag, a[2].imag, a[3].imag, a[4].imag, a[5].imag, a[6].imag, a[7].imag,
		a[8].imag, a[9].imag, a[10].imag, a[11].imag, a[12].imag, a[13].imag, a[14].imag, a[15].imag, 
		a[16].imag, a[17].imag, a[18].imag, a[19].imag, a[20].imag, a[21].imag, a[22].imag, a[23].imag,
		a[24].imag, a[25].imag, a[26].imag, a[27].imag, a[28].imag, a[29].imag, a[30].imag, a[31].imag };
    int16 b_real_v[16] _MM_ALIGN32 = { b[0].real, b[1].real, b[2].real, b[3].real, b[4].real, b[5].real, b[6].real, b[7].real,
		b[8].real, b[9].real, b[10].real, b[11].real, b[12].real, b[13].real, b[14].real, b[15].real,
		b[16].real, b[17].real, b[18].real, b[19].real, b[20].real, b[21].real, b[22].real, b[23].real,
		b[24].real, b[25].real, b[26].real, b[27].real, b[28].real, b[29].real, b[30].real, b[31].real };
    int16 b_imag_v[16] _MM_ALIGN32 = { b[0].imag, b[1].imag, b[2].imag, b[3].imag, b[4].imag, b[5].imag, b[6].imag, b[7].imag,
		b[8].imag, b[9].imag, b[10].imag, b[11].imag, b[12].imag, b[13].imag, b[14].imag, b[15].imag,
		b[16].imag, b[17].imag, b[18].imag, b[19].imag, b[20].imag, b[21].imag, b[22].imag, b[23].imag,
		b[24].imag, b[25].imag, b[26].imag, b[27].imag, b[28].imag, b[29].imag, b[30].imag, b[31].imag };
	__m512i a_real;
	__m512i a_imag;
	__m512i b_real;
	__m512i b_imag;
	//__m256i _mm256_load_si256 (__m256i const * mem_addr)
	//__m512i _mm512_load_si512 (void const* mem_addr)
	a_real = _mm512_load_si512((__m512i*)a_real_v);
	a_imag = _mm512_load_si512((__m512i*)a_imag_v);
	b_real = _mm512_load_si512((__m512i*)b_real_v);
	b_imag = _mm512_load_si512((__m512i*)b_imag_v);
	__m512i a_b_real;
	__m512i a_b_imag;
	__m512i a_real_b_imag;
	__m512i a_imag_b_real;
	//__m256i _mm256_mulhi_epi16 (__m256i a, __m256i b) choose the high 16bit
	//__m256i _mm256_mulhrs_epi16 (__m256i a, __m256i b) >>14 and choose the low 16bit
	//__m512i _mm512_mulhrs_epi16 (__m512i a, __m512i b)
	a_b_real = _mm512_mulhrs_epi16(a_real, b_real);
	a_b_imag = _mm512_mulhrs_epi16(a_imag, b_imag);
	a_real_b_imag = _mm512_mulhrs_epi16(a_real, b_imag);
	a_imag_b_real = _mm512_mulhrs_epi16(a_imag, b_real);
	//__m256i _mm256_sub_epi16 (__m256i a, __m256i b)
	//__m256i _mm256_add_epi16 (__m256i a, __m256i b)
	//__m512i _mm512_sub_epi16 (__m512i a, __m512i b)
	//__m512i _mm512_add_epi16 (__m512i a, __m512i b)
	__m512i dest_real;
	__m512i dest_imag;
	dest_real = _mm512_sub_epi16(a_b_real, a_b_imag);
	dest_imag = _mm512_add_epi16(a_real_b_imag, a_imag_b_real);
    int16 dest_real_v[32] _MM_ALIGN32;
    int16 dest_imag_v[32] _MM_ALIGN32;
	//_mm256_store_si256
	//void _mm512_store_si512 (void* mem_addr, __m512i a)
	_mm512_store_si512((__m512i*)dest_real_v,dest_real);
	_mm512_store_si512((__m512i*)dest_imag_v,dest_imag);
	int i = 0;
	for (i = 0; i < 32; i++){
		dest[i].real = dest_real_v[i]<<2;//m256i_i16
		dest[i].imag = dest_imag_v[i]<<2;
	}
}
*/

//#pragma pack()

//#define DEBUGAVX2
#ifdef DEBUGAVX2

#define NUMBER 16
#define NUMBER_2 32
int main(int argc, char* argv[]){
	/*printf("test for add:\n");
	test_256_float_add();
	printf("test for mult:\n");
	test_256_float_mult();*/
	//printf("size of complex32: %d\n",sizeof(complex32));
	complex32* a = (complex32*)malloc(NUMBER * sizeof(complex32));
	complex32* b = (complex32*)malloc(NUMBER * sizeof(complex32));
	complex32* c = (complex32*)malloc(NUMBER * sizeof(complex32));
	srand((unsigned)time(NULL));
	int j = 0;
	for (j = 0; j < NUMBER; j++){
		a[j].real = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		a[j].imag = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		b[j].real = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		b[j].imag = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
	}
	
	//mult __m256i
	//normal function
	printf("Normal function: mult\n");
	clock_t start_time = clock();
	int i = 0,times=0;
	for (times = 0; times < 10000000;times++)
	for (i = 0; i < NUMBER; i++){
		c[i]=multForComplex32(a[i],b[i]);
	}
	clock_t end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The mult answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c[i].real, c[i].imag);
	printf("use AVX function: mult\n");
	//using AVX
	start_time = clock();
	for (times = 0; times < 10000000; times++)
	//Mult_complex32Vector(a, b, c);
	Mult_complex32Vector_2(a, b, c);
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The mult answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c[i].real, c[i].imag);
	
	//add
	//normal function
	printf("Normal function: add\n");
	start_time = clock();
	for (times = 0; times < 10000000;times++)
	for (i = 0; i < NUMBER; i++){
		c[i]=addForComplex32(a[i],b[i]);
	}
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The add answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c[i].real, c[i].imag);
	printf("use AVX function: add\n");
	//using AVX
	start_time = clock();
	for (times = 0; times < 10000000; times++)
	Add_complex32Vector_2(a, b, c);
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The add answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c[i].real, c[i].imag);
	
	//sub
	//normal function
	printf("Normal function: sub\n");
	start_time = clock();
	for (times = 0; times < 10000000;times++)
	for (i = 0; i < NUMBER; i++){
		c[i]=reduceForComplex32(a[i],b[i]);
	}
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The sub answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c[i].real, c[i].imag);
	printf("use AVX function: sub\n");
	//using AVX
	start_time = clock();
	for (times = 0; times < 10000000; times++)
	Sub_complex32Vector_2(a, b, c);
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The sub answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c[i].real, c[i].imag);
	
	/*
	//mult __m512i
	complex32* a_2 = (complex32*)malloc(NUMBER_2 * sizeof(complex32));
	complex32* b_2 = (complex32*)malloc(NUMBER_2 * sizeof(complex32));
	complex32* c_2 = (complex32*)malloc(NUMBER_2 * sizeof(complex32));
	srand((unsigned)time(NULL));
	for (j = 0; j < NUMBER_2; j++){
		a_2[j].real = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		a_2[j].imag = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		b_2[j].real = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		b_2[j].imag = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
	}
	
	//normal function
	printf("Normal function: mult for __m512i\n");
	start_time = clock();
	for (times = 0; times < 10000000;times++)
	for (i = 0; i < NUMBER_2; i++){
		c_2[i]=multForComplex32(a_2[i],b_2[i]);
	}
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The mult answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c_2[i].real, c_2[i].imag);
	printf("use AVX function: mult\n");
	//using AVX
	start_time = clock();
	for (times = 0; times < 10000000; times++)
	Mult_complex32Vector_3(a_2, b_2, c_2);
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The mult answer:\n");
	for (i = 0; i < NUMBER / 2; i++) printf("%d %d\n", c_2[i].real, c_2[i].imag);
	*/	
	
	//Matrix_Mult
	complex32 h[Matrix_H][Matrix_H];
	complex32 x[Matrix_H];
	complex32* x_dest = (complex32*)malloc(Matrix_H * sizeof(complex32));
	memset(x_dest,0,4*sizeof(complex32));
	srand((unsigned)time(NULL));
	for(i=0;i<Matrix_H;i++){
		for (j = 0; j < Matrix_H; j++){
			h[i][j].real = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
			h[i][j].imag = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		}
		x[i].real = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
		x[i].imag = (double)(rand() / (double)RAND_MAX) * (0x1 << 13);
	}
	
	//Matrix mult __m256i
	//normal function
	printf("Normal function: Matrix mult\n");
	start_time = clock();
	for (times = 0; times < 10000000;times++)
		multForMatrix(h,x,x_dest);
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The matrix mult answer:\n");
	for (i = 0; i < Matrix_H; i++) printf("%d %d\n", x_dest[i].real, x_dest[i].imag);
	printf("use AVX function: mult Matrix\n");
	//using AVX
	start_time = clock();
	for (times = 0; times < 10000000; times++)
	//Mult_complex32Vector(a, b, c);
		Mult_Matrix_AVX2_4(h,x,x_dest);
	end_time = clock();
	printf("pall times = %fs\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
	printf("The matrix mult answer:\n");
	for (i = 0; i < Matrix_H; i++) printf("%d %d\n", x_dest[i].real, x_dest[i].imag);
	
	//free data
	free(a);
	free(b);
	free(c);
	
	//return to main
	return 0;
}

#endif
