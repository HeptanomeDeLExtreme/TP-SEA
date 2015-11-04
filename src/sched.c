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
	__asm("mov %0, lr" : "=r"(current_process->lr_user));
	__asm("mov %0, sp" : "=r"(current_process->sp));
	__asm("swi 0" : : : "lr");
	__asm("mov sp, %0" : : "r"(current_process->sp));
	__asm("mov lr, %0" : : "r"(current_process->lr_user));
}

void do_sys_yieldto()
{
	// Prochain process
	struct pcb_s* dest;
	dest = (struct pcb_s*)*(stackHead + OFFSET_R1);
		
	// On prend l'adresse présente dans *(StackHead + 1) qui représente
	// l'adresse mémoire où est stockée la sauvegarde des registres
	// du process où on souhaite se rendre
	
	uint32_t * src = stackHead;
	for(int i = 0; i<13 ;i++)
	{
		current_process->reg[i] = *(src+i);
		*(src+i) = dest->reg[i];
	}
	//current_process->lr_svc = *(src+13);
	
	// Récupérer le registre de statut et le placer en fin de pile
	//__asm("mrs r3, SPSR");
	//__asm("mov %0,r3" : "=r"(current_process->user_status)); 

	//__asm("cps 0x1F"); // system
	//__asm("mov %0,lr" : "=r"(current_process->lr_user));
	//__asm("cps 0x13"); // SVC

	__asm("mrs %0, SPSR" : "=r"(current_process->user_status));
	// On change de process : current = destination
	//current_process = dest;
	
	//__asm("cps 0x1F"); // system
	//__asm("mov lr,%0" : :  "r"(current_process->lr_user));
	//__asm("cps 0x13"); // SVC

	//__asm("mov r3,%0" : : "r"(current_process->user_status));
	//__asm("msr spsr,r3");
	
	src = stackHead;
	// On prend ce qu'il y a dans dest pcb et on le met dans StackHead
	for(int i = 0; i<13 ;i++)
	{
		//*(src+i) = dest->reg[i];
	}
	//*(src+13) = dest->lr_svc;
	__asm("msr SPSR, %0" : : "r"(dest->user_status));
	current_process = dest;
}
