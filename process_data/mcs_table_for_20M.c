#include "../headers/globalVarINIT.h"

void mcs_table_for_20M(unsigned char *rate_type, int *n_bpscs, int *n_dbps, int *n_cbps, int *n_es)
{
    unsigned char RATE_TYPE[10] = {0,0,1,0,1,2,1,5,1,5};
    int N_BPSCS[10] = {1,2,2,4,4,6,6,6,8,8};
    int N_ES[10] = {1,1,1,1,1,1,1,1,1,1};
    int N_DBPS[10] = {117, 234, 351, 468, 702, 936, 1053, 1170, 1404, 1560};
    int N_CBPS[10] = {234, 468, 468, 936, 936, 1404, 1404, 1404, 1872, 1872};

    int N_SS   = N_STS;
    int index  = MCS;
    *rate_type = RATE_TYPE[index];
    *n_bpscs   = N_BPSCS[index];
    *n_dbps    = N_SS*N_DBPS[index];
    *n_cbps    = N_SS*N_CBPS[index];
    *n_es      = N_ES[index];
}
