#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../../headers/commonStructure.h"
#include "../../headers/globalVarINIT.h"
#include "../../headers/integerTypeDef.h"
#include "../../headers/process_data.h"

#define twice(N) (1<<(N))

static int pilot_N = 8;
static int zero_N  = 14;

int16 *streamweave_table[N_STS]; /**< 含导频的分流交织表 */

//导频位置查询
static int pilot_index80[8] = {121,284,495,658,770,933,1144,1307};/**< 此处插入导频的坐标进行了修改，地址改到调制映射之前数据地址处 */
static int zero_index80[14] = {1,2,3,4,5,6,713,714,715,1422,1423,1424,1425,1426};

static complex32 Data_pilot20[4] = {{8192,0},{8192,0},{8192,0},{-8192,0}};
static complex32 Data_pilot40[6] = {{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0}};
static complex32 Data_pilot80[8] = {{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{8192,0}};
static complex32 Data_pilot160[8]= {{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{8192,0}};

static complex32 pilot_type[3]={{-8192,0},{0,0},{8192,0}};

void generate_stream_code_in_table(int16 **code_in,int N_BPSCS, int N_CBPS, int N_ES)                               //无导频位置的分流交织表
{
    int CodeLength = N_CBPS/N_STS;

    int16 *output = (int16 *)malloc(sizeof(int16)*(CodeLength*N_STS));
    int p;
    int temp=CodeLength*N_STS;
    for(p=0; p<temp; p++)
        output[p] = p;

    int i,j,k;
    int s,S,i_ss,k1;
    int floor1,floor2;
    int16 *BCC_stream_parser[N_STS];
    for(i=1;i<=N_STS;i++)
    {
        BCC_stream_parser[i-1] = (int16 *)malloc(sizeof(int16)*CodeLength);
        if(BCC_stream_parser[i-1]==NULL)
        {
            printf("error: BCC_stream_parser[%d]¡",i-1);
            exit(1);
        }
    }

    if(N_BPSCS/2>1)
        s=N_BPSCS/2;
    else
        s=1;
    S=N_STS*s;

    for(i_ss=1;i_ss<=N_STS;i_ss++)          //分流器
    {
        for(k=1;k<=CodeLength;k++)
        {
            k1=k-1;
            //floor1=k1/s;
            //j=floor1%N_ES+1;
            floor2=k1/N_ES/s;
            i=(i_ss-1)*s+S*floor2+k1%s+1;
            BCC_stream_parser[i_ss-1][k-1]=output[i-1];
        }
    }

    int index,r,J_ss;
    int N_CBPSS = N_CBPS/N_STS;
    int N_COL = 26;
    int N_ROW = 9*N_BPSCS;
    int N_ROT;
    int J[8]={0,5,2,7,3,6,1,4};

    if(N_STS>4)
        N_ROT = 28;
    else
        N_ROT = 58;

    for(k=1;k<=N_CBPSS;k++)
    {
        k1=k-1;
        floor1=k1/N_COL;
        i=N_ROW*(k1%N_COL)+floor1+1;
        for(i_ss=1;i_ss<=N_STS;i_ss++)
        {
            ///code_out[i_ss-1][i+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
            code_in[i_ss-1][i-1] = BCC_stream_parser[i_ss-1][k-1];
        }
    }

    for(k=1;k<=N_CBPSS;k++)
    {
        k1=k-1;
        floor1=k1/s;
        floor2=N_COL*k1/N_CBPSS;
        floor2=k1+N_CBPSS-floor2;
        j=s*floor1+floor2%s+1;
        for(i_ss=1;i_ss<=N_STS;i_ss++)
        {
            ///BCC_stream_parser[i_ss-1][j+(sym-1)*N_CBPSS-1] = code_out[i_ss-1][k+(sym-1)*N_CBPSS-1];
            BCC_stream_parser[i_ss-1][j-1] = code_in[i_ss-1][k-1];
        }
    }

    if(N_STS==1)
    {
        for(k=1;k<=N_CBPSS;k++)
        {
            for(i_ss=1;i_ss<=N_STS;i_ss++)
            {
                ///code_out[i_ss-1][k+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
                code_in[i_ss-1][k-1] = BCC_stream_parser[i_ss-1][k-1];
            }
    }
    }
    else if(N_STS>=5)
    {

        for(i_ss=1;i_ss<=N_STS;i_ss++)
        {
            J_ss=J[i_ss-1];
            for(k=1;k<=N_CBPSS;k++)
            {
                k1=k-1;
                floor1=k1-J_ss*N_ROT*N_BPSCS;
                while(floor1<0)
                {
                    floor1=floor1+N_CBPSS;
                }
                r=floor1+1;
                ///code_out[i_ss-1][r+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
                code_in[i_ss-1][r-1] = BCC_stream_parser[i_ss-1][k-1];
            }
        }

    }
    else
    {

        for(i_ss=1;i_ss<=N_STS;i_ss++)
        {
            floor1=(i_ss-1)*2;
            floor2=(i_ss-1)/3;
            index =(floor1%3+3*floor2)*N_ROT*N_BPSCS;
            for(k=1;k<=N_CBPSS;k++)
            {
                k1=k-1;
                floor1=k1-index;
                while(floor1<0)
                {
                    floor1=floor1+N_CBPSS;
                }
                r=floor1+1;
                ///code_out[i_ss-1][r+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
                code_in[i_ss-1][r-1] = BCC_stream_parser[i_ss-1][k-1];
            }
        }

    }

    for(i=1;i<=N_STS;i++)
    {
        free(BCC_stream_parser[i-1]);
        BCC_stream_parser[i-1] = NULL;
    }

}

void PilotAdd_table(int16 **code_in,int code_inLength,int16 **code_out,int code_outLength) //加入导频位置的分流交织表
{
    //新建拓展的二维数组
    //unsigned int code_out[N_STS][Length+8+14];
    int i,j,n,p,z;
    const int pilot_N = 8;
    const int zero_N  = 14;
    int Data_pilot80[pilot_N];
    for(i=0;i<3;i++) {Data_pilot80[i] = 6002;Data_pilot80[i+5] = 6002;}
    for(i=0;i<2;i++)
        Data_pilot80[i+3] = 6000;
    int zero=6001;

    n=0;p=0;z=0;
    for(j=0;j<code_outLength;j++)
    {

            if(((j+1)==pilot_index80[p])&&(p<pilot_N))//插入+1和—1的导频地址
            {
                for(i=0;i<N_STS;i++)
                   code_out[i][j]=Data_pilot80[p];
                p++;
            }
            else if(((j+1)==zero_index80[z])&&(z<zero_N))//插入零频地址
            {
                for(i=0;i<N_STS;i++)
                    code_out[i][j]=zero;
                z++;
            }
            else if(n<code_inLength)
            {
                for(i=0;i<N_STS;i++)
                    code_out[i][j]=code_in[i][n];
                n++;
            }
    }
}

void initial_streamwave_table()
{
    int i;

    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_CBPS, N_ES;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);

    unsigned int CodeLength = N_CBPS/N_STS;
    unsigned int TableLength = CodeLength+pilot_N+zero_N;

    /**< 生成无导频的分流交织表 */
    int16 *code_in_table[N_STS];
    for(i=0;i<N_STS;i++)
    {
        code_in_table[i] = (int16 *)malloc(sizeof(int16)*CodeLength);
        if(code_in_table[i]==NULL)
        {
            printf("error: process_data//code_out[%d]",i);
            exit(1);
        }
    }
    generate_stream_code_in_table(code_in_table,N_BPSCS,N_CBPS,N_ES);
    /**< 生成含有导频地址的分流交织表 */
    for(i=0;i<N_STS;i++)
    {
        streamweave_table[i] = (int16 *)malloc(sizeof(int16)*TableLength);
        if(streamweave_table[i]==NULL)
        {
            printf("error: process_data//code_out[%d]",i);
            exit(1);
        }
    }
    PilotAdd_table(code_in_table,CodeLength, streamweave_table, TableLength);

    for(i=0;i<N_STS;i++)
    {
        free(code_in_table[i]);
        code_in_table[i] = NULL;
    }

}

void parser_stream_interweave(unsigned char *output,unsigned char **stream_interweave_dataout,int16 **stream_parser_weave_table)               //查表函数
{
    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_CBPS, N_ES;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);
    int N_service = 16;
    int N_tail = 6;
    int N_SYM = ceil(((double)(8*APEP_LEN + N_service + N_tail*N_ES) / (double)N_DBPS));
    int N_CBPSS = N_CBPS/N_STS;
    int TableLength = N_CBPSS+pilot_N+zero_N;
    int i,j,k;
    for(k=0; k<N_SYM; k++)
    {
        for(i=0;i<N_STS;i++)
        {

            for(j=0;j<TableLength;j++)
            {
                if(stream_parser_weave_table[i][j]==6002)stream_interweave_dataout[i][j+k*TableLength]=101;
                else if(stream_parser_weave_table[i][j]==6001)stream_interweave_dataout[i][j+k*TableLength]=100;
                else if(stream_parser_weave_table[i][j]==6000)stream_interweave_dataout[i][j+k*TableLength]=99;
                else
                {
                    stream_interweave_dataout[i][j+k*TableLength] = output[stream_parser_weave_table[i][j]+k*N_CBPS];
                }
            }
        }
    }

}


void __bi2de_opt(unsigned char **code_out,int mode,int num,int Nov_STS)
{
      int j,k;
      int out;
      int pilot_num=0;
      for(j=1;j<=num;j++)
      {
          out = 0;
          for(k=1;k<=mode;k++)
          {
              if(code_out[Nov_STS][mode*(j-1-pilot_num)+pilot_num+k-1]==99||
                 code_out[Nov_STS][mode*(j-1-pilot_num)+pilot_num+k-1]==100||
                 code_out[Nov_STS][mode*(j-1-pilot_num)+pilot_num+k-1]==101)
              {
                 out=code_out[Nov_STS][mode*(j-1-pilot_num)+pilot_num+k-1];
                 pilot_num++;
                 break;
              }
              else if(code_out[Nov_STS][mode*(j-1-pilot_num)+pilot_num+k-1] == 1)
                    out = out + twice(mode-k);
              else
                    out = out + 0;
          }
          code_out[Nov_STS][j-1] = out;
      }

}

void __Modulation_11ax_opt(unsigned char **code_out, int mode,int num, complex32 **sym_mod,int Nov_STS)
{
    int index[16] = {1,2,4,3,8,7,5,6,16,15,13,14,9,10,12,11};
    int j,real_j,imag_j,k;
    //int N = (*NumSampEffect)/2;
    complex32 *C;
    /////////////////////////////////////////////////////////////
    int N_CBPS, N_SYM, ScrLength, valid_bits;
    GenInit(&N_CBPS, &N_SYM, &ScrLength, &valid_bits);
    /////////////////////////////////////////////////////////////
    switch(mode)
    {
        case 0: C=QAM1;
                break;

        case 1: C=QAM2;
                break;

        case 2: C=QAM4;
                break;

        case 3: C=QAM8;
                break;

        case 4: C=QAM16;
                break;

        default: printf("error: value of mode is wrong!");
                 exit(1);
    }
    int t=0;
    for(j=1;j<=num;j++)
    {

        if(code_out[Nov_STS][j-1]>16)
        {
            //sym_mod[Nov_STS][t]=pilot_type[code_out[Nov_STS][j-1]-99];
            (*sym_mod)[Nov_STS*subcar*N_SYM + t]=pilot_type[code_out[Nov_STS][j-1]-99];
        }
        else
        {
           real_j = index[code_out[Nov_STS][j-1]];
           imag_j = index[code_out[Nov_STS][j]];
           k = (twice(mode)-imag_j)*twice(mode)+real_j;
           //sym_mod[Nov_STS][t] = C[k-1];
           (*sym_mod)[Nov_STS*subcar*N_SYM + t] = C[k-1];
           j++;
        }
        t++;
    }

}

void modulate_mapping(unsigned char *BCC_output, complex32 **subcar_map_data)
{
	//printf("use Optmodulate!\n");

    int i;
    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_CBPS, N_ES;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);
    int N_service = 16;
    int N_tail = 6;
    int N_SYM = ceil(((double)(8*APEP_LEN + N_service + N_tail*N_ES) / (double)N_DBPS));
    int mode = N_BPSCS/2;
    int num =(N_CBPS/mode/N_STS+pilot_N+zero_N)*N_SYM;
    unsigned int CodeLength = N_CBPS/N_STS;
    unsigned int TableLength = CodeLength+pilot_N+zero_N;

    /**< 通过查表生成分流交织后的数据块 */
    unsigned char *stream_interweave_dataout[N_STS];
    for(i=0;i<N_STS;i++)
    {
        stream_interweave_dataout[i] = (unsigned char *)malloc(sizeof(unsigned char)*TableLength*N_SYM);
        if(stream_interweave_dataout[i]==NULL)
        {
            printf("error: process_data//code_out[%d]",i);
            exit(1);
        }
    }

    parser_stream_interweave(BCC_output, stream_interweave_dataout, streamweave_table);

/*    FILE *c = fopen("stream_interweave_dataout.txt","wt");
    for(j=0;j<TableLength*N_SYM;j++)
    {
        for(i=0;i<N_STS;i++)
            fprintf(c,"%d\t",stream_interweave_dataout[i][j]);
        fprintf(c,"\n");
    }*/

    /**< 进行调制映射 */
    int Nov_STS;
    if(mode == 0)
    {
        printf("sorry: mode = 0, we haven't done this.");
        exit(0);
    }
    for(Nov_STS=0; Nov_STS<N_STS; Nov_STS++)
    {
        /**< 将二进制转化为十进制 */
        __bi2de_opt(stream_interweave_dataout, mode,num,Nov_STS);
        /**< 星座映射和插入导频 */
        __Modulation_11ax_opt(stream_interweave_dataout, mode, num, subcar_map_data, Nov_STS);
    }
    //modulation_mapping(stream_interweave_dataout, N_SYM, subcar_map_data, Nov_STS) ;

   for(i=0;i<N_STS;i++)
   {
       free(stream_interweave_dataout[i]);
       stream_interweave_dataout[i] = NULL;
   }
}

#ifdef DEBUGMODULATIONOPT
int main()
{
    int i,j;

    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_CBPS, N_ES;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);
    int N_service = 16;
    int N_tail = 6;
    int N_SYM = ceil(((double)(8*APEP_LEN + N_service + N_tail*N_ES) / (double)N_DBPS));
    unsigned int CodeLength = N_CBPS/N_STS;
    unsigned int TableLength = CodeLength+pilot_N+zero_N;
    unsigned char *BCC_output=(unsigned char*)malloc(N_SYM*N_CBPS);
    memset(BCC_output,0,sizeof(unsigned char)*N_SYM*N_CBPS);

    BCC_output[0]=1;
    BCC_output[1]=1;
    BCC_output[2]=1;
    BCC_output[3]=1;
    BCC_output[98]=1;
    BCC_output[205]=1;
    BCC_output[312]=1;
    BCC_output[410]=1;
    BCC_output[517]=1;
    BCC_output[624]=1;
    BCC_output[722]=1;
    BCC_output[829]=1;
    BCC_output[936]=1;

    BCC_output[98+N_CBPS]=1;
    BCC_output[205+N_CBPS]=1;
    BCC_output[312+N_CBPS]=1;
    BCC_output[410+N_CBPS]=1;
    BCC_output[517+N_CBPS]=1;
    BCC_output[624+N_CBPS]=1;
    BCC_output[722+N_CBPS]=1;
    BCC_output[257+N_CBPS]=1;
    BCC_output[829+N_CBPS]=1;
    BCC_output[936+N_CBPS]=1;

    BCC_output[3150]=1;
    BCC_output[2685]=1;
    BCC_output[257]=1;
    BCC_output[3150+N_CBPS]=1;
    BCC_output[2685+N_CBPS]=1;
    BCC_output[257+N_CBPS]=1;
    FILE *t=fopen("BCC_out.txt","wt");
    for(i=0;i<N_SYM*N_CBPS;i++)
    {
        fprintf(t,"%d\n",BCC_output[i]);
    }

    /**< 初始化分流交织表(含导频) */
    initial_streamwave_table();

    /**< 调制映射 */
    complex32 *subcar_map_data[N_STS];
    for(i=0;i<N_STS;i++)
    {
        subcar_map_data[i] = (complex32 *)malloc(sizeof(complex32)*(subcar*N_SYM));
        if(subcar_map_data[i]==NULL)
        {
            printf("error: error: process_data//subcar_map_data[%d]",i);
            exit(1);
        }
    }
    modulate_mapping(BCC_output,subcar_map_data);

    free(BCC_output);
    BCC_output = NULL;
    return 0;
}
#endif
