#ifndef ___SYSCALL_H___ 
# define ___SYSCALL_H___


/* User Side*/
void sys_reboot();


/* Handler */
void swi_handler();

/* Kerner Side */
void do_sys_reboot();

/* Extern declaration */
extern void PUT32 ( unsigned int, unsigned int );


#endif
