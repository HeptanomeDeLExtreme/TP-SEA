#include "syscall.h"
#include "util.h"

//Main
void 
__attribute__((naked))
swi_handler(){
	//Sauvegarde de contexte maggle
	__asm("stmfd sp!, {r0-r12, lr}");

	//Récupération du numéro d'appel système
	int numAppel;
	__asm("mov %0, r0" : "=r"(numAppel): :"r0");

	if(numAppel == '1'){
		do_sys_reboot();
	}
	else if(numAppel == '2'){
		do_sys_nop();
	}

	else if(numAppel == '3'){
		uint64_t date_ms;
		__asm("mov %0, r1" : "=r"(date_ms): :"r1");
		void do_sys_settime(date_ms);
	}
	else{
		PANIC();
	}

	//Restauration de contexte maggle
	__asm("ldmfd sp!, {r0-r12, pc}^");
}

void sys_reboot(){
	__asm("mov r0, %0" : : "r"('1') : "r0");
	__asm("SWI #0");
}

void sys_nop(){
	__asm("mov r0, %0" : : "r"('2') : "r0");
	__asm("SWI #0");
}

void sys_settime(uint64_t date_ms){
	__asm("mov r0, %0" : : "r"('3') : "r0");
	__asm("mov r1, %0" : : "r"(date_ms) : "r1");
	__asm("SWI #0");
}

void do_sys_reboot(){
	__asm("b 0");
}

void do_sys_nop(){
//Ne fait rien
}

void do_sys_settime(uint64_t date_ms){

}

