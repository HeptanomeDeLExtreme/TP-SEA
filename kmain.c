#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "fb.h"

void UsbInitialise();
void KeyboardUpdate();
char KeyboardGetChar();

void runloop()
{
	KeyboardUpdate();
	
	char c = KeyboardGetChar();
	
	if (c != 0)
	{
		drawChar(c);
	}
}


int kmain (void)
{
	hw_init();
	sched_init(); 

	create_process((func_t*)&runloop);

	timer_init();
	ENABLE_IRQ();
	
	// init keyboard
	UsbInitialise();
	
	// init screen
	FramebufferInitialize();
	
	// clean screen
	clear();
	
	__asm("cps 0x10");


	// draw console hello world
	drawHelloConsole();

	return 0;
}
