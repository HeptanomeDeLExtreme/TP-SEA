#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "fb.h"

void UsbInitialise();
void KeyboardUpdate();
char KeyboardGetChar();

char commandLine[256];
int commandLineSize = 0;

void applyMethod()
{
	newLine();
	for(int i = 0;i<commandLineSize;i++)
	{
		drawChar(commandLine[i]);
	}
	commandLineSize = 0;
	newLine();
	drawChar('>');
	drawChar(' ');
}

void keyboardLoop()
{
	while(1){
		KeyboardUpdate();
		
		char c = KeyboardGetChar();
		
		if (c != 0)
		{
			if(c=='\n')
			{
				applyMethod();
			}
			else{
				commandLine[commandLineSize] = c;
				commandLineSize++;
				drawChar(c);
			}
		}
	}
}

void printHeader()
{
	drawHelloConsole();
	while(1)
	{
		volatile int i = 0;
		i++;
	}
}

void dummy()
{
	while(1)
	{
		volatile int i = 0;
		i++;
	}
}

int kmain (void)
{
	sched_init(); 
	
	create_process((func_t*)&printHeader);
	create_process((func_t*)&dummy);
	create_process((func_t*)&keyboardLoop);
	

	// init screen
	FramebufferInitialize();
	
	// init keyboard
	UsbInitialise();
	
	// clean screen
	clear();
	
	hw_init();
	
	
	timer_init();
	ENABLE_IRQ();
	
	__asm("cps 0x10");
	
	while(1)
	{
		sys_yield();
	}

	return 0;
}
