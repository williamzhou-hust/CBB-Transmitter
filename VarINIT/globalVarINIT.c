#include "../headers/commonStructure.h"
#include "../headers/globalVarINIT.h"

#ifndef GLOBALVAL
#define GLOBALVAL

int dotscale = 13;
int	BW = 0;
int	Band = 80;
int	SampRate = 40;
int	MCS = 5;
int	APEP_LEN = 512;//data length
int	delta_f = 0;
int lIsMu = 0;//not MU
int	CpLen = 128;//3.2 * SampRate;
int CodeTypeIsBcc = 1;//is BCC
char* ChannelMode = "AWGN";

//data
int subcar = 256;
int N_tone = 242;
int N_pilot = 8;

complex32 regForAddWin[N_TX][3];

complex32 basicSTF[53] = {{0,0},{0,0},{5792,5792},{0,0},{0,0},{0,0},{-5793,-5793},{0,0},{0,0},{0,0},{5792,5792},\
                         {0,0},{0,0},{0,0},{-5793,-5793},{0,0},{0,0},{0,0},{-5793,-5793},{0,0},{0,0},\
                         {0,0},{5792,5792},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{-5793,-5793},\
                         {0,0},{0,0},{0,0},{-5793,-5793},{0,0},{0,0},{0,0},{5792,5792},{0,0},{0,0},\
                         {0,0},{5792,5792},{0,0},{0,0},{0,0},{5792,5792},{0,0},{0,0},{0,0},{5792,5792},{0,0},{0,0}};

complex32 basicLTF[53] = {{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},\
                         {8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},\
                         {-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{0,0},{8192,0},{-8192,0},\
                         {-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{-8192,0},\
                         {-8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{-8192,0},\
                         {8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0}};

int CyclicShiftTable[36] = {0, 0, -200, 0, -100, -200, 0, -50, -100, -150,\
                            0, -175, -25, -50, -75, 0, -200, -25, -150, -175, -125,\
                            0, -200, -150, -25, -175, -75, -50, 0, -175, -150, -125,\
                            -25, -100, -50, -200};

//HE LTF
complex32 basicHeLTF[256]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{8192,0},{8192,0},{-8192,0},{-8192,0},\
                           {8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},\
                           {8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},\
                           {8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},\
                           {-8192,0},{8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},\
                           {-8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},\
                           {-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},\
                           {8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},\
                           {8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},\
                           {8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},\
                           {8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},\
                           {-8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},\
                           {8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{0,0},{0,0},{0,0},\
                           {8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},\
                           {-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},\
                           {-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},\
                           {8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},\
                           {-8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},\
                           {-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{-8192,0},\
                           {8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},\
                           {-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},\
                           {8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},\
                           {8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},\
                           {-8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},{8192,0},\
                           {8192,0},{-8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},\
                           {8192,0},{0,0},{0,0},{0,0},{0,0},{0,0}};

int pilot_index[8] = {25, 53, 89, 117, 139, 167, 203, 231};

//P matrix
complex32 P1[1]={{8192,0}};
complex32 P2[4]={{8192,0},{-8192,0},{8192,0},{8192,0}};
complex32 P3[12]={{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},\
                   {8192,0},{8192,0},{8192,0},{-8192,0}};

complex32 P4[16]={{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},\
                   {8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{8192,0}};

complex32 P5[30]={{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-4097,7094},\
                   {-4096,-7095},{-8192,-1},{-4097,7094},{-4096,-7095},{8192,0},{4095,7094},{-4097,7094},{8192,0},\
                   {-4096,-7095},{4096,-7095},{8192,0},{8192,0},{8192,0},{-8193,-1},{8192,0},{8192,0},\
                   {8192,0},{4096,-7095},{-4096,-7095},{8192,0},{-4097,7094},{4095,7094}};

complex32 P6[36]={{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-4097,7094},\
                   {-4096,-7095},{-8192,-1},{-4097,7094},{-4096,-7095},{8192,0},{4095,7094},{-4097,7094},{8192,0},\
                   {-4096,-7095},{4096,-7095},{8192,0},{8192,0},{8192,0},{-8193,-1},{8192,0},{8192,0},\
                   {8192,0},{4096,-7095},{-4096,-7095},{8192,0},{-4097,7094},{4095,7094},{8192,0},{-4096,-7095},\
                   {-4097,7094},{-8193,-1},{-4096,-7095},{-4097,7094}};

complex32 P7[56]={{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},\
                   {8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},\
                   {8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},\
                   {-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},\
                   {8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},\
                   {8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},\
                   {8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0}};

complex32 P8[64]={{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},\
                   {8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},\
                   {8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},\
                   {-8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{8192,0},{8192,0},\
                   {8192,0},{-8192,0},{8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{-8192,0},\
                   {8192,0},{8192,0},{-8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{-8192,0},\
                   {8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{-8192,0},{-8192,0},{8192,0},\
                   {-8192,0},{8192,0},{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{-8192,0}};

complex32* P[8]={P1,P2,P3,P4,P5,P6,P7,P8};

//CSD Table for HeLTF
int T_iss_CS1[1] = {0};
int T_iss_CS2[2] = {0 , -400};
int T_iss_CS3[3] = {0 , -400 , -200};
int T_iss_CS4[4] = {0 , -400 , -200 , -600};
int T_iss_CS5[5] = {0 , -400 , -200 , -600 , -350};
int T_iss_CS6[6] = {0 , -400 , -200 , -600 , -350 , -650};
int T_iss_CS7[7] = {0 , -400 , -200 , -600 , -350 , -650 , -100};
int T_iss_CS8[8] = {0 , -400 , -200 , -600 , -350 , -650 , -100 , -750};

int* shiftTableForHeLTF[8]={T_iss_CS1,T_iss_CS2,T_iss_CS3,T_iss_CS4,T_iss_CS5,T_iss_CS6,T_iss_CS7,T_iss_CS8};

#endif

