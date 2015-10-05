#include "stdint.h"
#include "sched.h"

extern uint64_t* stackHead;
struct pcb_s* current_process;
struct pcb_s kmain_process;

void sched_init()
{
	current_process = &kmain_process;
}

void sys_yieldto(struct pcb_s* dest)
{
	/* Switch to interrupt mode */
	__asm("mov r0, %0" : : "r"(5));
	__asm("swi 0" : : : "lr");
}

void do_sys_yieldto()
{
	// Sauver le PCB actuel
	__asm("mov %0, r0" : : "r"(current_process->reg[0]));
	__asm("mov %0, r1" : : "r"(current_process->reg[1]));
	__asm("mov %0, r2" : : "r"(current_process->reg[2]));
	__asm("mov %0, r3" : : "r"(current_process->reg[3]));
	__asm("mov %0, r4" : : "r"(current_process->reg[4]));
	__asm("mov %0, r5" : : "r"(current_process->reg[5]));
	__asm("mov %0, r6" : : "r"(current_process->reg[6]));
	__asm("mov %0, r7" : : "r"(current_process->reg[7]));
	__asm("mov %0, r8" : : "r"(current_process->reg[8]));
	__asm("mov %0, r9" : : "r"(current_process->reg[9]));
	__asm("mov %0, r10" : : "r"(current_process->reg[10]));
	__asm("mov %0, r11" : : "r"(current_process->reg[11]));
	__asm("mov %0, r12" : : "r"(current_process->reg[12]));
	//~ __asm("mov %0, cpsr" : : "r"(current_process->cpsr));
	__asm("mov %0, lr" : : "r"(current_process->lr));
	__asm("mov %0, pc" : : "r"(current_process->pc));
	__asm("mov %0, sp" : : "r"(current_process->sp));
	
	// Le prochain process
	struct pcb_s* dest;
	dest = *(stackHead + 1);
	//~ 
	//~ // Charger le PCB du prochain process
	__asm("mov r0, %0" : : "r"(dest->reg[0]));
	__asm("mov r1, %0" : : "r"(dest->reg[1]));
	__asm("mov r2, %0" : : "r"(dest->reg[2]));
	__asm("mov r3, %0" : : "r"(dest->reg[3]));
	__asm("mov r4, %0" : : "r"(dest->reg[4]));
	__asm("mov r5, %0" : : "r"(dest->reg[5]));
	__asm("mov r6, %0" : : "r"(dest->reg[6]));
	__asm("mov r7, %0" : : "r"(dest->reg[7]));
	__asm("mov r8, %0" : : "r"(dest->reg[8]));
	__asm("mov r9, %0" : : "r"(dest->reg[9]));
	__asm("mov r10, %0" : : "r"(dest->reg[10]));
	__asm("mov r11, %0" : : "r"(dest->reg[11]));
	__asm("mov r12, %0" : : "r"(dest->reg[12]));
	//~ __asm("mov cpsr, %0" : : "r"(dest->cpsr));
	__asm("mov lr, %0" : : "r"(dest->lr));
	__asm("mov pc, %0" : : "r"(dest->pc));
	__asm("mov sp, %0" : : "r"(dest->sp));
	//~ 
	current_process = dest;
}
