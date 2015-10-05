#include "stdint.h"

extern uint64_t* stackHead;
struct pcb_s* current_process;

void sys_yieldto(struct pcb_s* dest)
{
	/* Switch to interrupt mode */
	__asm("mov r0, %0" : : "r"(5));
	__asm("swi 0" : : : "lr");
}

void do_sys_yieldto()
{
	// Sauver le PCB
	__asm("mov %0, r0" : : "r"(r[0]));
	__asm("mov %0, r1" : : "r"(r[1]));
	__asm("mov %0, r2" : : "r"(r[2]));
	__asm("mov %0, r3" : : "r"(r[3]));
	__asm("mov %0, r4" : : "r"(r[4]));
	__asm("mov %0, r5" : : "r"(r[5]));
	__asm("mov %0, r6" : : "r"(r[6]));
	__asm("mov %0, r7" : : "r"(r[7]));
	__asm("mov %0, r8" : : "r"(r[8]));
	__asm("mov %0, r9" : : "r"(r[9]));
	__asm("mov %0, r10" : : "r"(r[10]));
	__asm("mov %0, r11" : : "r"(r[11]));
	uint64_t dest;
	dest = *(stackHead + 1);
}
