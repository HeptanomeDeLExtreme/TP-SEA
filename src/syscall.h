#ifndef ___SYSCALL_H___ 
# define ___SYSCALL_H___

#include "stdint.h"

/* User Side*/
void sys_reboot();
void sys_nop();
void sys_settime();
uint64_t sys_gettime();
void sys_yieldto(struct pcb_s* dest);

/* Handler */
void swi_handler();

/* Kerner Side */
void do_sys_reboot();
void do_sys_nop();
void do_sys_settime();
void do_sys_gettime();
void do_sys_yieldto();

/* Extern declaration */
//extern void SET32 ( unsigned int, unsigned int );


#endif
