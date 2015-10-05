#include "stdint.h"

extern uint64_t* stackHead;

void sys_yieldto(struct pcb_s* dest)
{
	/* Switch to interrupt mode */
	__asm("mov r0, %0" : : "r"(5));
	__asm("swi 0" : : : "lr");
}

void do_sys_yieldto()
{
	uint64_t dest;
	dest = *(stackHead + 1);
}
