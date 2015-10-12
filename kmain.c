#include"util.h"
#include"syscall.h"
#include"sched.h"

struct pcb_s pcb1, pcb2;
struct pcb_s *p1, *p2;

void user_process_1()
{
	int
	v1=5;
	while(1)
	{
		v1++;
		sys_yieldto(p2);
	}
}

void user_process_2()
{
	int v2=-12;
	while(1)
	{
		v2-=2;
		sys_yieldto(p1);
	}
}

void kmain(void)
{
	sched_init();
	p1=&pcb1;
	p2=&pcb2;
	// initialize p1 and p2//
	// [ ton code va la ]
	
	p1->lr_user = &(user_process_1);
	p2 ->lr_user = &(user_process_2);
	
	__asm("cps 0x10");
	// switch CPU to USER mode//
	// **********************************************************************
	sys_yieldto(p1);
	// this is now unreachable
	PANIC();
}
