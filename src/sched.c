#include "stdint.h"
#include "sched.h"

extern uint32_t* stackHead;
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
	__asm("mov r1, %0" : : "r"(dest));
	__asm("swi 0" : : : "lr");
}

void do_sys_yieldto()
{
	// Prochain process
	struct pcb_s* dest;
	dest = stackHead + OFFSET_R1;
		
	// On prend l'adresse présente dans *(StackHead + 1) qui représente
	// l'adresse mémoire où est stockée la sauvegarde des registres
	// du process où on souhaite se rendre
	uint32_t * src = stackHead;
	current_process->user_status = *(src++);
	for(int i = 0; i<14 ;i++)
	{
		current_process->reg[i] = *(src++);
	}
	src--; // to compensate something
	current_process->sp = *(src++);
	current_process->lr = *(src++);
	current_process->pc = *(src++);
	
	// On change de process : current = destination
	current_process = dest;
	
	// On prend ce qu'il y a dans dest pcb et on le met dans StackHead
	src = stackHead;
	*(src++) = dest->user_status;
	for(int i = 0; i<14 ;i++)
	{
		*(src++) = dest->reg[i];;
	}
	*(src++) = dest->sp;
	*(src++) = dest->lr;
	*(src++) = dest->pc;

}
