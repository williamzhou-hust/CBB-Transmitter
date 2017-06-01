#ifndef PROCESS_DATA
#define PROCESS_DATA
//#include "../allHeaders.h"
#include "commonStructure.h"

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
#ifdef OPTIMIZATION
extern void Creatnewchart();
extern void BCC_encoder_OPT(unsigned char *data_scramble, int ScrLength, int N_SYM, unsigned char **code_out, int CodeLength);
#endif
//���ƺ���
extern void modulate(unsigned char **code_out , int BCC_length, int N_SYM, complex32 **sym_mod, int *NumSampEffect );
extern void initial_streamwave_table();
#ifndef DPDK_FRAME  //no  working in  dpdk frame.
void modulate_mapping(unsigned char *BCC_output, complex32 **subcar_map_data);
#else
void modulate_mapping(unsigned char *BCC_output, unsigned char **stream_interweave_dataout, complex32 **subcar_map_data);
#endif 
//���뵼Ƶ��Ƶ
extern void PilotAdd_SubcarMap(complex32 **sym_mod, int N_SYM, complex32 **subcar_map_data);
//CSD
extern void Data_CSD(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data);
#ifdef AVX2
extern void __Data_CSD_aux(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data,int NTXindex);//maybe use for multi pthread
#endif
#endif // PROCESS_DATA
