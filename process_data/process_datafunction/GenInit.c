#include <math.h>
#include "../../headers/globalVarINIT.h"
#include "../../headers/process_data.h"

void GenInit(int *N_CBPS, int *N_SYM, int *ScrLength, int *valid_bits)
{
    unsigned char rate_type;
    int N_BPSCS, N_DBPS, N_ES,PSDU_LENGTH, MAC_PAD, N_PAD;
    mcs_table_for_20M(&rate_type, &N_BPSCS, &N_DBPS, N_CBPS, &N_ES);
    int N_service = 16;
    int N_tail = 6;

    *N_SYM = ceil(((double)(8*APEP_LEN + N_service + N_tail*N_ES) / (double)N_DBPS));
    PSDU_LENGTH = ((*N_SYM)*N_DBPS - N_service - N_tail*N_ES)/8;
    MAC_PAD = (PSDU_LENGTH - APEP_LEN)*8;
    N_PAD = (*N_SYM)*N_DBPS - 8*PSDU_LENGTH - N_service - N_tail*N_ES;

    *ScrLength = APEP_LEN+2+(MAC_PAD+N_PAD)/8+1;   //加1保证末尾有字节数个零
    *valid_bits = (APEP_LEN+2)*8+MAC_PAD+N_PAD;
}
