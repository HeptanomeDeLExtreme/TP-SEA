#ifndef ___SCHED_H___ 
# define ___SCHED_H___

#include "stdint.h"

#define NUMBER_REGISTER 13
#define OFFSET_R0 1 // in order to have R0
#define OFFSET_R1 2 // in order to have R1
#define OFFSET_R2 3 // in order to have R2

/* Structure definitions */
struct pcb_s {
	uint32_t reg[NUMBER_REGISTER];
	uint32_t lr_svc;
	uint32_t lr_user;
	uint32_t user_status;
};

/* Scheduling */
void sched_init();

/* User Side*/
void sys_yieldto(struct pcb_s* dest);

/* Kernel Side */
void do_sys_yieldto();

#endif
