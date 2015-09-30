#include "syscall.h"
#include "util.h"

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

void swi_handler()
{
	int interruptNumber;
	__asm("mov %0, r0" : "=r"(interruptNumber));
	
	switch(interruptNumber)
	{
		case 1 :
			do_sys_reboot();
			break;	
		case 2 :
			do_sys_nop();
			break;
		default :	
			PANIC();

	}

}

void do_sys_nop()
{

}

void do_sys_reboot()
{
	//const int PM_RSTC = 0x2010001c;
	//const int PM_WDOG = 0x20100024;
	//const int PM_PASSWORD = 0x5a000000;
	//const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;
	//PUT32(PM_WDOG, PM_PASSWORD | 1);
	//PUT32(PM_RSTC, PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET);
	//while(1);

	// Methode gitan parce que l'autre marche pas
	__asm("mov pc, %0" : : "r"(0x8000));
}

