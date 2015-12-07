#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "fb.h"

void UsbInitialise();
void KeyboardUpdate();
char KeyboardGetChar();

volatile int i = 65;

void runloop()
{
	while(1){
		KeyboardUpdate();
		
		char c = KeyboardGetChar();
		
		if (c != 0)
		{
			drawChar(c);
		}
	}
}

void bash()
{
	// draw console hello world
	drawHelloConsole();
	
	while(1)
	{
		i = 65;
		i++;
		if(i>127)
		{
			i = 65;
		}
	}
}

void oho()
{
	drawChar(i);
}


int kmain (void)
{
	hw_init();
	sched_init(); 

	create_process((func_t*)&runloop);
	create_process((func_t*)&bash);
	create_process((func_t*)&oho);


	timer_init();

	
	// init screen
	FramebufferInitialize();
	
	// init keyboard
	//~ UsbInitialise();
	
	// clean screen
	clear();
	
	ENABLE_IRQ();
	__asm("cps 0x10");


	return 0;
}
