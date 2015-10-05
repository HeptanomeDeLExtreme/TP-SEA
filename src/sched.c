#include "sched.h"
#include "syscall.h"

void sys_yieldto(struct pcb_s* dest){
	__asm("mov r0, %0" : : "r"(YIELDTO) : "r0");
	__asm("mov r1, %0" : : "r"(dest) : "r1");
	__asm("SWI #0");
}

void do_sys_yieldto(){
	__asm("mov %0, r0" : "=r"(current_process.reg0): :"r0");
	__asm("mov %0, r1" : "=r"(current_process.reg1): :"r1");
	__asm("mov %0, r2" : "=r"(current_process.reg2): :"r2");
	__asm("mov %0, r3" : "=r"(current_process.reg3): :"r3");
	__asm("mov %0, r4" : "=r"(current_process.reg4): :"r4");
	__asm("mov %0, r5" : "=r"(current_process.reg5): :"r5");
	__asm("mov %0, r6" : "=r"(current_process.reg6): :"r6");
	__asm("mov %0, r7" : "=r"(current_process.reg7): :"r7");
	__asm("mov %0, r8" : "=r"(current_process.reg8): :"r8");
	__asm("mov %0, r9" : "=r"(current_process.reg9): :"r9");
	__asm("mov %0, r10" : "=r"(current_process.reg10): :"r10");
	__asm("mov %0, r11" : "=r"(current_process.reg11): :"r11");
	__asm("mov %0, r12" : "=r"(current_process.reg12): :"r12");
	__asm("mov %0, r13" : "=r"(current_process.reg13): :"r13");
	__asm("mov %0, r14" : "=r"(current_process.reg14): :"r14");
	__asm("mov %0, pc" : "=r"(current_process.regPC): :"pc");
	__asm("mov %0, cpsr" : "=r"(current_process.regCPSR): :"cpsr");

	pcb_s* dest = *(top_of_stack+1);

	__asm("mov r0, %0" : : "r"(dest.reg0) : "r0");
	__asm("mov r1, %0" : : "r"(dest.reg1) : "r1");
	__asm("mov r2, %0" : : "r"(dest.reg2) : "r2");
	__asm("mov r3, %0" : : "r"(dest.reg3) : "r3");
	__asm("mov r4, %0" : : "r"(dest.reg4) : "r4");
	__asm("mov r5, %0" : : "r"(dest.reg5) : "r5");
	__asm("mov r6, %0" : : "r"(dest.reg6) : "r6");
	__asm("mov r7, %0" : : "r"(dest.reg7) : "r7");
	__asm("mov r8, %0" : : "r"(dest.reg8) : "r8");
	__asm("mov r9, %0" : : "r"(dest.reg9) : "r9");
	__asm("mov r10, %0" : : "r"(dest.reg10) : "r10");
	__asm("mov r11, %0" : : "r"(dest.reg11) : "r11");
	__asm("mov r12, %0" : : "r"(dest.reg12) : "r12");
	__asm("mov r13, %0" : : "r"(dest.reg13) : "r13");
	__asm("mov r14, %0" : : "r"(dest.reg14) : "r14");
	__asm("mov pc, %0" : : "r"(dest.regPC) : "pc");
	__asm("mov cpsr, %0" : : "r"(dest.regCPSR) : "cpsr");


}
