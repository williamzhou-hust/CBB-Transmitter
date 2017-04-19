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

    free(data_scramble);
    data_scramble = NULL;
    ///////////////////////////////////////////////////////////////////////////////////
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

    PilotAdd_SubcarMap(sym_mod, N_SYM, subcar_map_data);

    for(i=0;i<N_STS;i++)
    {
        free(sym_mod[i]);
        sym_mod[i] = NULL;
    }
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
    Data_CSD(subcar_map_data, N_SYM, csd_data);

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
