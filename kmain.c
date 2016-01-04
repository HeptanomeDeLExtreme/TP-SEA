#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "pwm.h"
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

char history[256];
int historySize = 0;

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
		if(index != commandLineSize)
		{
			command[commandSize] = commandLine[index];
			commandSize++;
			index++;
		}
		else
		{
			break;
		}
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
	// OK echo : Affiche à l'écran ce qui est tapé.
	// OK music : Lance audio_test qui joue de la musique.
	// xx currentTime
	// xx reboot
	int resultEcho = strcmp("echo", command);
	int resultMusic = strcmp("music", command);
	
	if(resultEcho == 0)
	{
		drawString(parameters, parametersSize);
	}
	else if(resultMusic == 0) 
	{
		drawString("Quelle douce musique...", 23);
		audio_test();
	}
	else
	{
		drawError("La commande n'existe pas.", 25);
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
				// sauvegarde de l'historique
				//~ drawChar(commandLine[0]);
				//~ drawChar(command[0]);
				for(int i = 0; i<commandLineSize;i++)
				{
					history[i] = commandLine[i];
					historySize++;
				}
				
				newLine();
				applyMethod();
			}
			else if(c == 'h')
			{
				// restauration de l'historique
				for(int i = 0; i<historySize;i++)
				{
					commandLine[i] = history[i];
				}
				commandLineSize = historySize;
				newLine();
				drawChar('$');
				drawChar(' ');
				for(int i = 0; i<commandLineSize;i++)
				{
					drawChar(commandLine[i]);
				}
				
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
