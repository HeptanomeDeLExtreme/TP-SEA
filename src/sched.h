#ifndef ___SCHED_H___ 
# define ___SCHED_H___

#include "stdint.h"

/* User Side*/
void sys_yieldto(struct pcb_s* dest);

/* Kerner Side */
void do_sys_yieldto();

/* Structure definitions */
struct pcb_s {

};



#endif
