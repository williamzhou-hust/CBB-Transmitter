#ifndef HEADGLOBALVAR
#define HEADGLOBALVAR

#include "../headers/commonStructure.h"
#include "../../headers/integerTypeDef.h"
#define N_STS 4
#define N_TX 4
#define N_RX 4
#define PI 3.14159265359;
#define MKSUREENMEM(p) if(p==NULL){\
        printf("Not enougth memory!\n");\
        exit(1);}

extern int  dotscale;
extern int	BW;
extern int	Band;
extern int	SampRate;
extern int	MCS;
extern int	APEP_LEN;//data length
extern int	delta_f;
extern int  lIsMu;//not MU
extern int	CpLen;
extern int  CodeTypeIsBcc;//is BCC
extern char* ChannelMode;

extern complex32 regForAddWin[N_TX][3];
extern complex32 basicSTF[53];
extern complex32 basicLTF[53];
extern int CyclicShiftTable[36];

//HE LTF
extern complex32 basicHeLTF[256];
extern int pilot_index[8];
extern complex32* P[8];
extern int* shiftTableForHeLTF[8];

//Preamble and HeLTF
extern complex32* STF[N_TX];
extern complex32* LTF[N_TX];
extern complex32* Sig[N_TX];
extern complex32* heLTF[N_TX];
//data
extern int subcar;
extern int N_tone;
extern int N_pilot;

#ifdef AVX2
extern int csdTableForPreambleFlag;
extern complex32 csdTableForPreamble[N_TX][64];
extern int csdTableForHeLTFFlag;
extern complex32 csdTableForHeLTF[N_TX][256];
#endif

#ifdef OPTIMIZATION
typedef struct BCCStruct{
unsigned char output[16];
unsigned char endstate;
}BCCStruct_t;
extern BCCStruct_t BCCTable[256][128];
typedef struct Scramble
{
    unsigned char data_out;
    unsigned char reg_out;
}Scramble_t;
extern Scramble_t ChartTable[256][128];
#endif

//interleaver table
extern int16 *streamweave_table[N_STS]; /**< 含导频的分流交织表 */
extern complex32 QAM1[2];
extern complex32 QAM2[4];
extern complex32 QAM4[16];
extern complex32 QAM8[64];
extern complex32 QAM16[256];

#endif // HEADGLOBALVAR
