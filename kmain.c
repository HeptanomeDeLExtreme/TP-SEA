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

void testAffichage()
{
	for(int i = 0; i < commandLineSize; i++)
	{
		drawChar(commandLine[i]);
	}
}

void resetCommand(int sizeCommand)
{
	sizeCommand = 0;
}

void addCharacterInString(char c, char* str, int strSize)
{
	str[strSize] = c;
	strSize++;
}

void commandProcess()
{
	int index = 0;
	char c = commandLine[index];
	testAffichage();
	while(c != ' ')
	{
		addCharacterInString(commandLine[index], command, commandSize);
		index++;
	}
	drawString(command, commandSize);
	newLine();
	
	index++;
	c = commandLine[index];
	while(c == ' ')
	{
		index++;
	}

	for(int i = index; i < commandLineSize; i++)
	{
		addCharacterInString(commandLine[index], parameters, parametersSize);
		index++;
	}
	drawString(parameters, parametersSize);
	newLine();

	resetCommand(commandSize);
	resetCommand(parametersSize);
}

void applyMethod()
{
	newLine();
	drawString("---applyMethod---", 17);
	newLine();
	testAffichage();
	commandProcess();
	resetCommand(commandLineSize);
	newLine();
	drawString("---applyMethod---", 17);
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
			if(c == '\n')
			{
				applyMethod();
			}
			else{
				addCharacterInString(c, commandLine, commandLineSize);
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
