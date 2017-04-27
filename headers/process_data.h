#ifndef PROCESS_DATA
#define PROCESS_DATA
#include "../allHeaders.h"

#define twice(N) (1<<(N))

//�������ݵ�������
extern void GenerateData(unsigned char *databits, complex32 **csd_data);

//MCS�����
extern void mcs_table_for_20M(unsigned char *rate_type, int *N_BPSCS, int *N_DBPS, int *N_CBPS, int *N_ES);

//�������ݱ���,ͬʱ��������
extern void GenInit(int *N_CBPS, int *N_SYM, int *ScrLength, int *valid_bits);//�������ݶ��ֽڳ��Ⱥ���Ч������
extern void GenDataAndScramble(unsigned char *data_scramble, int ScrLength, unsigned char *databits, int valid_bits); //���ݼ��ţ������ĸߵ�λ�ߵ�

//BCC����
extern void BCC_encoder(unsigned char *data_scramble, int ScrLength, int N_SYM, unsigned char **code_out, int CodeLength);
//���ƺ���
extern void modulate(unsigned char **code_out , int BCC_length, int N_SYM, complex32 **sym_mod, int *NumSampEffect );
//���뵼Ƶ��Ƶ
extern void PilotAdd_SubcarMap(complex32 **sym_mod, int N_SYM, complex32 **subcar_map_data);
//CSD
#ifndef AVX2
extern void Data_CSD(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data);
#else
extern void Data_CSD(complex32 **subcar_map_data, int N_SYM, int iss, complex32 **csd_data);
#endif

#endif // PROCESS_DATA
