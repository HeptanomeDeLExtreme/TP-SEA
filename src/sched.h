#ifndef ___SCHED_H___ 
# define ___SCHED_H___

#include "stdint.h"

#define NUMBER_REGISTER 12;

/* User Side*/
void sys_yieldto(struct pcb_s* dest);

/* Kerner Side */
void do_sys_yieldto();

/* Structure definitions */
struct pcb_s {
	uint32_t reg[NUMBER_REGISTER];
	uint32_t sp;
	uint32_t lr;
	uint32_t pc;
	uint32_t cpsr;
};



#endif
