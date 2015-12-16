#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "fb.h"

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1==*s2))
        s1++,s2++;
    return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

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
	drawChar(commandLine[0]);
}

void commandProcess()
{
	// Récupération de la commande et du reste dans deux variables
	// différentes
	int index = 0;
	while(commandLine[index] != ' ')
	{
		command[commandSize] = commandLine[index];
		commandSize++;
		index++;
	}
	command[commandSize] = '\0';
	commandSize++;
	
	index++;
	while(commandLine[index] == ' ')
	{
		index++;
	}

	for(int i = index; i < commandLineSize; i++)
	{
		parameters[parametersSize] = commandLine[index];
		parametersSize++;
		index++;
	}
	parameters[parametersSize] = '\0';
	parametersSize++;
	
	// Traitement de chaque commande individuellement
	int resultEcho = strcmp("echo", command);
	
	if(resultEcho == 0)
	{
		drawString(parameters, parametersSize);
	}
	else
	{
		drawString("La commande n'existe pas.", 25);
	}
	
	commandSize = 0;
	parametersSize = 0;
}

void applyMethod()
{
	commandProcess();
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
			if(c == '\n')
			{
				newLine();
				applyMethod();
			}
			else
			{
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
