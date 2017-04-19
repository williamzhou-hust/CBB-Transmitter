#ifndef PROCESS_DATA
#define PROCESS_DATA
#include "../allHeaders.h"

#define twice(N) (1<<(N))

//生成数据的主函数
extern void GenerateData(unsigned char *databits, complex32 **csd_data);

//MCS查表函数
extern void mcs_table_for_20M(unsigned char *rate_type, int *N_BPSCS, int *N_DBPS, int *N_CBPS, int *N_ES);

//生成数据比特,同时进行扰码
extern void GenInit(int *N_CBPS, int *N_SYM, int *ScrLength, int *valid_bits);//计算数据段字节长度和有效比特数
extern void GenDataAndScramble(unsigned char *data_scramble, int ScrLength, unsigned char *databits, int valid_bits); //数据加扰，出来的高低位颠倒

//BCC编码
extern void BCC_encoder(unsigned char *data_scramble, int ScrLength, int N_SYM, unsigned char **code_out, int CodeLength);
//调制函数
extern void modulate(unsigned char **code_out , int BCC_length, int N_SYM, complex32 **sym_mod, int *NumSampEffect );
//插入导频零频
extern void PilotAdd_SubcarMap(complex32 **sym_mod, int N_SYM, complex32 **subcar_map_data);
//CSD
extern void Data_CSD(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data);

#endif // PROCESS_DATA
