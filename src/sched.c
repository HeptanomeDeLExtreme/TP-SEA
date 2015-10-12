#include "sched.h"
#include "syscall.h"

void sched_init(){
	current_process = &kmain_process;
}

void sys_yieldto(struct pcb_s* dest){
	__asm("mov r0, %0" : : "r"(YIELDTO) : "r0");
	__asm("mov r1, %0" : : "r"(dest) : "r1");
	__asm("SWI #0");
}

void do_sys_yieldto(){
	int i=0;

	//Sauvegarde du contexte utilisateur mis dans la pile par le handler
	for(i=0 ; i < 13 ; i++){
		current_process->reg[i]=*(top_of_stack + i);
	}
	
	/* The particular case of lr_svc */
	current_process->lr_svc=*(top_of_stack + 13);
	/* The particular case of spsr */
	__asm("mrs r0, SPSR");
	__asm("mov %0, r0" : "=r"(current_process->cpsr): :"r0");


	//The particular case of lr_user
	//Passer en mode system
	__asm("cps 0b11111");
	__asm("mov lr, %0" : : "r"(current_process->lr_user) : "lr");

	//Passer en mode SVC
	__asm("cps 0b10011");


	//Restauration du contexte utilisateur dans la pile
	struct pcb_s* dest = (void*) *(top_of_stack+1);
	current_process = dest;

	int j=0;
	for(j=0 ; j < 13 ; j++){
		*(top_of_stack+j)=dest->reg[j];
	}
	__asm("mov r0, %0" : : "r"(current_process->cpsr) : "r0");
	__asm("msr SPSR, r0");
	*(top_of_stack + 13) = current_process->lr_svc;
}
