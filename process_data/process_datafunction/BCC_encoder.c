#include<stdio.h>
#include<stdlib.h>
#include "../../headers/globalVarINIT.h"
#include "../../headers/process_data.h"
#include "../../headers/process.h"

void BCC_encoder(unsigned char *data_scramble, int ScrLength, int N_SYM, unsigned char **code_out, int CodeLength)
{
    unsigned int numbytes = ScrLength +1;
    unsigned char *inPtr  = data_scramble;
    unsigned char *outPtr;
    unsigned char *output = (unsigned char *)malloc(sizeof(unsigned char)*(CodeLength*N_STS+24));  //24为防止指针越界

    outPtr = output;

    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_CBPS, N_ES;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);

    //BCC need init, it have init in sig process
    ccodedot11_encode (numbytes, inPtr, outPtr, rate_type);

    int i,j,k;
    int s,S,i_ss,k1;
    int floor1,floor2;
    unsigned char *BCC_stream_parser[N_STS];
    for(i=1;i<=N_STS;i++)
    {
        BCC_stream_parser[i-1] = (unsigned char *)malloc(sizeof(unsigned char)*CodeLength);
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

    int sym;
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

    for(sym=1;sym<=N_SYM;sym++)
    {
        for(k=1;k<=N_CBPSS;k++)
        {
            k1=k-1;
            floor1=k1/N_COL;
            i=N_ROW*(k1%N_COL)+floor1+1;
            for(i_ss=1;i_ss<=N_STS;i_ss++)
            {
                code_out[i_ss-1][i+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
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
                BCC_stream_parser[i_ss-1][j+(sym-1)*N_CBPSS-1] = code_out[i_ss-1][k+(sym-1)*N_CBPSS-1];
            }
        }

        if(N_STS==1)
        {
            for(k=1;k<=N_CBPSS;k++)
            {
                for(i_ss=1;i_ss<=N_STS;i_ss++)
                {
                    code_out[i_ss-1][k+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
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
                    code_out[i_ss-1][r+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
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
                    code_out[i_ss-1][r+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
                }
            }

        }
    }

    for(i=1;i<=N_STS;i++)
    {
        free(BCC_stream_parser[i-1]);
        BCC_stream_parser[i-1] = NULL;
    }

}

#ifdef OPTIMIZATION
void BCC_encoder_OPT(unsigned char *data_scramble, int ScrLength, int N_SYM, unsigned char **code_out, int CodeLength)
{
    unsigned int numbytes = ScrLength +1;
    unsigned char *inPtr  = data_scramble;
    unsigned char *outPtr;
    unsigned char *output = (unsigned char *)malloc(sizeof(unsigned char)*(CodeLength*N_STS+24));  //24为防止指针越界

    outPtr = output;

    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_CBPS, N_ES;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, &N_CBPS, &N_ES);

    //BCC need init, it have init in sig process
    //ccodedot11_encode (numbytes, inPtr, outPtr, rate_type);
	unsigned char initstate=0;
	check_BCCcode(numbytes,inPtr,outPtr,initstate,rate_type);
	*code_out=output;

	/*
    int i,j,k;
    int s,S,i_ss,k1;
    int floor1,floor2;
    unsigned char *BCC_stream_parser[N_STS];
    for(i=1;i<=N_STS;i++)
    {
        BCC_stream_parser[i-1] = (unsigned char *)malloc(sizeof(unsigned char)*CodeLength);
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

    int sym;
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

    for(sym=1;sym<=N_SYM;sym++)
    {
        for(k=1;k<=N_CBPSS;k++)
        {
            k1=k-1;
            floor1=k1/N_COL;
            i=N_ROW*(k1%N_COL)+floor1+1;
            for(i_ss=1;i_ss<=N_STS;i_ss++)
            {
                code_out[i_ss-1][i+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
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
                BCC_stream_parser[i_ss-1][j+(sym-1)*N_CBPSS-1] = code_out[i_ss-1][k+(sym-1)*N_CBPSS-1];
            }
        }
        if(N_STS==1)
        {
            for(k=1;k<=N_CBPSS;k++)
            {
                for(i_ss=1;i_ss<=N_STS;i_ss++)
                {
                    code_out[i_ss-1][k+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
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
                    code_out[i_ss-1][r+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
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
                    code_out[i_ss-1][r+(sym-1)*N_CBPSS-1] = BCC_stream_parser[i_ss-1][k+(sym-1)*N_CBPSS-1];
                }
            }

        }
    }

    for(i=1;i<=N_STS;i++)
    {
        free(BCC_stream_parser[i-1]);
        BCC_stream_parser[i-1] = NULL;
    }
*/
}


#endif
