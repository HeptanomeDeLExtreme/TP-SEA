#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "fb.h"
#include "string.h"

void UsbInitialise();
void KeyboardUpdate();
char KeyboardGetChar();

char commandLine[256];
int commandLineSize = 0;
char command[256];
int commandSize = 0;
char parameters[256];
int parametersSize = 0;

// echo
// music
// currentTime
// reboot

void resetCommand()
{
	commandLineSize = 0;
}

void addCharacterInString(char c, char* str, int strSize)
{
	str[strSize] = c;
	strSize++;
}

void commandProcess()
{
	int indexParameter = 0;
	while(c != ' ')
	{
		addCharacterInString(commandLine[index], command, commandSize);
	}
	addCharacterInString('\0', command, commandSize);
	
	indexParameter = commandSize + 1;
	while(c != '\0')
	{
		addCharacterInString(commandLine[indexParameter], parameters, parametersSize);
		indexParameter;
	}
	addCharacterInString('\0', parameters, parametersSize);
	
	drawString(command, commandSize - 1); // Without /0
	drawChar('\n');
	drawString('ZBLAH !', 7);
	drawString(parameters, parametersSize - 1);
}

void applyMethod()
{
	addCharacterInString('\0', commandLine, commandLineSize);
	newLine();
	/*for(int i = 0;i<commandLineSize;i++)
	{
		drawChar(commandLine[i]);
	}*/
	commandProcess();
	resetCommand();
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
				addCharacterInString(c);
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
