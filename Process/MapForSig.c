#include "../headers/commonStructure.h"
#include "../headers/process.h"
#include "../headers/globalVarINIT.h"

void MapForSig(unsigned char* sigAfterBCC, complex32* sigAfterMap, int length){
        while(length){
            int16 sigdata=*sigAfterBCC;
            sigAfterMap->real=(2*sigdata-1)<<dotscale;
            ++sigAfterBCC;
            ++sigAfterMap;
            --length;
        }
}
