#include "../../headers/commonStructure.h"
#include "../../headers/globalVarINIT.h"

int pilot_index20[4] = {12,26,40,54};
int zero_index20[8]  = {1,2,3,4,33,62,63,64};

int pilot_index40[6] = {12,40,54,76,90,118};
int zero_index40[14] = {1,2,3,4,5,6,64,65,66,124,125,126,127,128};

int pilot_index80[8] = {26,54,90,118,140,168,204,232};
int zero_index80[14] = {1,2,3,4,5,6,128,129,130,252,253,254,255,256};

int pilot_index160[16] = {26,54,90,118,140,168,204,232,282,310,346,374,396,424,460,488};
int zero_index160[28]  = {1,2,3,4,5,6,132,133,134,252,253,254,255,256,257,258,259,260,261,262,379,380,381,508,509,510,511,512};

complex32 Data_pilot20[4] = {{8192,0},{8192,0},{8192,0},{-8192,0}};
complex32 Data_pilot40[6] = {{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0}};
complex32 Data_pilot80[8] = {{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{8192,0}};
complex32 Data_pilot160[8]= {{8192,0},{8192,0},{8192,0},{-8192,0},{-8192,0},{8192,0},{8192,0},{8192,0}};

void PilotAdd_SubcarMap(complex32 **sym_mod, int N_SYM, complex32 **subcar_map_data)
{
    int pilot_N = 8;
    int zero_N  = 14;
    int N_tone_edff = N_tone - N_pilot;
    complex32 Data_pilot[pilot_N];
    complex32 Data_zero = {0,0};

    int sym;
    int circshift;
    int i,j,n,p,z;
    for(sym=1;sym<=N_SYM;sym++)
    {
        circshift = (sym-1)%N_pilot;
        for(j=0;j<pilot_N;j++)
        {
            Data_pilot[j] = Data_pilot80[(j+circshift)%pilot_N];
        }

        n=0;p=0;z=0;
        for(j=0;j<subcar;j++)
        {
            if(((j+1)==pilot_index80[p])&&(p<pilot_N))
            {
                for(i=0;i<N_STS;i++)
                    subcar_map_data[i][j+(sym-1)*subcar] = Data_pilot[p];
                p++;
            }
            else if(((j+1)==zero_index80[z])&&(z<zero_N))
            {
                for(i=0;i<N_STS;i++)
                    subcar_map_data[i][j+(sym-1)*subcar] = Data_zero;
                z++;
            }
            else
            {
                for(i=0;i<N_STS;i++)
                    subcar_map_data[i][j+(sym-1)*subcar] = sym_mod[i][n+(sym-1)*N_tone_edff];
                n++;
            }
        }

    }

}

