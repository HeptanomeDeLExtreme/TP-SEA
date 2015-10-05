#include "syscall.h"
#include "util.h"
#include "stdint.h"
#include "hw.h"

uint32_t* top_of_stack;

//Main
void 
__attribute__((naked))
swi_handler(){
	//Sauvegarde de contexte maggle
	__asm("stmfd sp!, {r0-r12, lr}");
	__asm("mov %0, sp" : "=r"(top_of_stack): :"sp");

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
		do_sys_settime();
	}

	else if(numAppel == '4'){
		do_sys_gettime();
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

	uint32_t date_lowbits = date_ms & (0x00000000FFFFFFFF);
  	uint32_t date_highbits = date_ms >> 32;
	__asm("mov r0, %0" : : "r"('3') : "r0");
	__asm("mov r1, %0" : : "r"(date_lowbits) : "r1");
	__asm("mov r2, %0" : : "r"(date_highbits) : "r2");
	__asm("SWI #0");
}

uint64_t sys_gettime(){
	__asm("mov r0, %0" : : "r"('4') : "r0");
	__asm("SWI #0");
	uint32_t date_lowbits;
  	uint64_t date_highbits;
	__asm("mov %0, r0" : "=r"(date_lowbits): :"r0");
	__asm("mov %0, r1" : "=r"(date_highbits): :"r1");
	uint64_t date_ms = (date_highbits << 32) | date_lowbits;
	return date_ms;
}

void do_sys_reboot(){
	__asm("b 0");
}

void do_sys_nop(){
//Ne fait rien
}

void do_sys_settime()
{
	uint64_t date_highbits = (uint64_t) *(top_of_stack+2);
	uint32_t date_lowbits = *(top_of_stack+1);
	uint64_t date_ms = (date_highbits << 32) | date_lowbits;
	set_date_ms(date_ms);
}

void do_sys_gettime(){
	uint64_t date_ms = get_date_ms();
	uint32_t date_lowbits = date_ms & (0x00000000FFFFFFFF);
  	uint32_t date_highbits = date_ms >> 32;

	*(top_of_stack) = date_lowbits;
	*(top_of_stack+1) = date_highbits;

}

