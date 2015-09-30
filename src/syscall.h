#ifndef ___SYSCALL_H___ 
# define ___SYSCALL_H___


/* User Side*/
void sys_reboot();
void sys_nop();

/* Handler */
void swi_handler();

/* Kerner Side */
void do_sys_reboot();
void do_sys_nop();

/* Extern declaration */
//extern void SET32 ( unsigned int, unsigned int );


#endif
