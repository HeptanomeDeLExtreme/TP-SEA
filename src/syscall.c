#include "syscall.h"
#include "util.h"
#include "stdint.h"
#include "hw.h"

/* Global definitions */
uint64_t* stackHead;

/* User side definitions */
void sys_reboot()
{
	__asm("mov r0, %0" : : "r"(1)); // Fill r0 with the number of the syscall
	__asm("SWI 0" : : : "lr"); // Software Interrupt
}

void sys_nop()
{
	__asm("mov r0, %0" : : "r"(2));
	__asm("swi 0" : : : "lr");
}

void sys_settime(uint64_t date_ms)
{
	__asm("mov r0, %0" : : "r"(3));
	__asm("mov r1, %0" : : "r"(date_ms));
	__asm("swi 0" : : : "lr");
}

uint64_t sys_gettime()
{
	/* Local variable */
	uint64_t reg0;
	//~ uint64_t reg1;
	//~ uint64_t time;
	
	/* Switch to interrupt mode */
	__asm("mov r0, %0" : : "r"(4));
	__asm("swi 0" : : : "lr");
	
	/* Get the result of do_sys_gettime() */
	__asm("mov %0, r0" : "=r"(reg0));
	//~ __asm("mov %0, r1" : "=r"(reg1));
	//~ 
	//~ /* Rebuild the result*/
	//~ time = reg1 << 32;
	//~ time += reg0;
	
	return reg0;
}

/* Handler definitions */
void __attribute__((naked)) swi_handler()
{

	__asm("stmfd sp!,{r0-r12,lr}"); // Save the user register
	
	// Mémoriser l'emplacement du sommet de la pile
	__asm("mov %0, sp" : "=r"(stackHead) : : "r0", "r1"); // Les
	// registres précieux sont R0 et R1.
		
	int interruptNumber;
	__asm("mov %0, r0" : "=r"(interruptNumber) : : "r0");
	
	switch(interruptNumber)
	{
		case 1 :
			do_sys_reboot();
			break;	
		case 2 :
			do_sys_nop();
			break;
		case 3 :
			do_sys_settime();
			break;
		case 4 :
			do_sys_gettime();
			break;
		default :	
			PANIC();

	}
	
	__asm("ldmfd sp!, {r0-r12,pc}^"); // Restore the user register
	
}

/* Kernel side implementation */
void do_sys_nop()
{
	
}

void do_sys_reboot()
{
	//const int PM_RSTC = 0x2010001c;
	//const int PM_WDOG = 0x20100024;
	//const int PM_PASSWORD = 0x5a000000;
	//const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;
	//SET32(PM_WDOG, PM_PASSWORD | 1);
	//SET32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
	//while(1);

	// Methode gitan parce que l'autre marche pas
	__asm("mov pc, %0" : : "r"(0x8000));
}

void do_sys_settime()
{
	// Récupérer le premier paramètre après le stackHead
	// qui représente le paramètre passé à la fonction sys_settime()
	uint64_t date_ms;
	date_ms = *(stackHead + 1);
	set_date_ms(date_ms);
}

void do_sys_gettime()
{
	uint64_t time = get_date_ms();
	*stackHead = time;
}
