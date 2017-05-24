#include<stdio.h>
#include<stdlib.h>
#include "../headers/globalVarINIT.h"
#include "../headers/commonStructure.h"
#include "../headers/process_data.h"

void GenerateData(unsigned char *databits, complex32 **csd_data)
{
    int i;
    int N_CBPS, N_SYM, ScrLength, valid_bits;
    ///////////////////////////////////////////////////////////////////////////////////
    GenInit(&N_CBPS, &N_SYM, &ScrLength, &valid_bits);
    unsigned char *data_scramble = (unsigned char *)malloc(sizeof(unsigned char)*ScrLength);
    GenDataAndScramble(data_scramble, ScrLength, databits, valid_bits);
    //////////////////////////////////////////////////////////////////////////////////
    unsigned char *code_out[N_STS];
    int CodeLength = N_SYM*N_CBPS/N_STS;

	#ifndef OPTIMIZATION
    for(i=0;i<N_STS;i++)
    {
        code_out[i] = (unsigned char *)malloc(sizeof(unsigned char)*CodeLength);
        if(code_out[i]==NULL)
        {
            printf("error: process_data//code_out[%d]",i);
            exit(1);
        }
    }

    BCC_encoder(data_scramble, ScrLength, N_SYM, code_out, CodeLength);
	#else
	//printf("use OPT BCC!\n");
	//unsigned char* BCCencodeout;
    unsigned char *BCCencodeout = (unsigned char *)malloc(sizeof(unsigned char)*(CodeLength*N_STS+24));  //24为防止指针越界
    BCC_encoder_OPT(data_scramble, ScrLength, N_SYM, &BCCencodeout, CodeLength);
	#endif

    free(data_scramble);
    data_scramble = NULL;
    ///////////////////////////////////////////////////////////////////////////////////
	#ifndef OPTIMIZATION
    complex32 *sym_mod[N_STS];
    for(i=0;i<N_STS;i++)
    {
        sym_mod[i] = (complex32 *)malloc(sizeof(complex32)*(CodeLength));
        if(sym_mod[i]==NULL)
        {
            printf("error: process_data//sym_mod[%d]¡",i);
            exit(1);
        }
    }
    int symod_lentgh;
    modulate(code_out, CodeLength, N_SYM, sym_mod, &symod_lentgh);
    for(i=0;i<N_STS;i++)
    {
        free(code_out[i]);
        code_out[i] = NULL;
    }
	#endif
    /////////////////////////////////////////////////////////////////////////////
    complex32 *subcar_map_data[N_STS];                                           //为插入导频后的数据申请空间
    for(i=0;i<N_STS;i++)
    {
        subcar_map_data[i] = (complex32 *)malloc(sizeof(complex32)*(subcar*N_SYM));
        if(subcar_map_data[i]==NULL)
        {
            printf("error: error: process_data//subcar_map_data[%d]¡",i);
            exit(1);
        }
    }
	#ifndef OPTIMIZATION
    PilotAdd_SubcarMap(sym_mod, N_SYM, subcar_map_data);
    for(i=0;i<N_STS;i++)
    {
        free(sym_mod[i]);
        sym_mod[i] = NULL;
    }
	#endif

	#ifdef OPTIMIZATION
	modulate_mapping(BCCencodeout,subcar_map_data);
	free(BCCencodeout);
	#endif

    //////////////////////////////////////////////////////////////////////////////
/*    complex32 *csd_data[N_STS];                                           //为CSD后的数据申请空间
    for(i=0;i<N_STS;i++)
    {
        csd_data[i] = (complex32 *)malloc(sizeof(complex32)*(subcar*N_SYM));
        if(csd_data[i]==NULL)
        {
            printf("error: process_data//csd_data[%d]¡",i);
            exit(1);
        }
    }
*/
	#ifndef AVX2
    Data_CSD(subcar_map_data, N_SYM, csd_data);
	#else
	for(i=0;i<N_STS;i++){
		__Data_CSD_aux(subcar_map_data, N_SYM, csd_data,i);
	}
	#endif
    for(i=0;i<N_STS;i++)
    {
        free(subcar_map_data[i]);
        subcar_map_data[i] = NULL;
    }
    //////////////////////////////////////////////////////////////////////////
/*
    int j;
    FILE *b;                                                                //将结果写入文件
    if((b=fopen("csd_data.txt","wt"))==NULL)
    {
        printf("error: b!");
        exit(1);
    }
    else
    {
        for(i=1;i<=N_STS;i++)
        {
            for(j=1;j<=(subcar*N_SYM);j++)
                fprintf(b,"%d\n",csd_data[i-1][j-1].imag);
        }

    }
    fclose(b);
*/
/*    for(i=0;i<N_STS;i++)
    {
        free(csd_data[i]);
        csd_data[i] = NULL;
    }
*/
}
