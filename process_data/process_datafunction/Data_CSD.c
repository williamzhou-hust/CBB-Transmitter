#include <math.h>
#include "../../headers/process_data.h"
#include "../../headers/globalVarINIT.h"
#include "../../headers/commonStructure.h"
#include "../../headers/process.h"
#ifndef AVX2
void Data_CSD(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data)
{
    int i,j,n;
    int det_F_ax = 78125;
    int T_iss_CS[8] = {0,-400,-200,-600,-350,-650,-100,-750};
    double phase,real_j,imag_j,pi=PI;
    complex32 exp_table[subcar];

    for(i=0;i<N_STS;i++)
    {
        phase = (-2)*pi*T_iss_CS[i]*det_F_ax/1000000000;
        for(j=(-subcar/2);j<=(subcar/2-1);j++)
        {
            real_j = cos(phase*j);
            imag_j = sin(phase*j);
            exp_table[j+subcar/2].real = real_j*(1<<(13));
            exp_table[j+subcar/2].imag = imag_j*(1<<(13));
        }

        for(n=0;n<N_SYM;n++)
        {
            for(j=0;j<subcar;j++)
            {
                csd_data[i][j+n*subcar] = multForComplex32(subcar_map_data[i][j+n*subcar],exp_table[j]);
            }
        }
    }
}
#else
void Data_CSD(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data){
	if(!csdTableForHeLTFFlag)
		initcsdTableForHeLTF();
	int i=0,j=0,n=0;
	for(i=0;i<N_STS;i++){
		for(n=0;n<N_SYM;n++){
			for(j=0;j<16;j++){
				Mult_complex32Vector_2(&subcar_map_data[i][n*256]+j*16,&csdTableForHeLTF[i][j*16],&csd_data[i][n*256]+j*16);
			}
		}
	}
}

void __Data_CSD_aux(complex32 **subcar_map_data, int N_SYM, complex32 **csd_data,int NTXindex){
	if(!csdTableForHeLTFFlag)
		initcsdTableForHeLTF();
	int j=0,n=0;
	for(n=0;n<N_SYM;n++){
		for(j=0;j<16;j++){
			//Mult_complex32Vector_2(&subcar_map_data[NTXindex][n*256]+j*16,&csdTableForHeLTF[NTXindex][j*16],&csd_data[NTXindex][n*256]+j*16);
                                       Mult_complex32Vector_2(&((*subcar_map_data)[NTXindex*subcar*N_SYM + n*256])+j*16,&csdTableForHeLTF[NTXindex][j*16], &((*csd_data)[NTXindex*subcar*N_SYM + n*256])+j*16);
		}
	}
}
#endif
