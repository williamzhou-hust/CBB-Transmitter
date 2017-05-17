
#ifndef _SIMPLE_MP_COMMANDS_H_
#define _SIMPLE_MP_COMMANDS_H_
extern const unsigned string_size;
// extern const unsigned NUM_MBUFS;
extern struct rte_ring *Ring_Beforescramble,*Ring_scramble_2_BCC,*Ring_BCC_2_modulation,*Ring_modulation_2_CSD,*Ring_AfterCSD;

extern struct rte_mempool *NUM_MBUFS;

extern volatile int quit;
#endif /* _SIMPLE_MP_COMMANDS_H_ */

