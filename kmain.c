#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"
#include "fb.h"


void USPiEnvClose();
int USPiKeyboardAvailable();
int USPiEnvInitialize();
int USPiInitialize();
void USPiKeyboardRegisterKeyPressedHandler();

static void KeyPressedHandler (const char *pString)
{
	drawString("coucou",6);
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
	

	// init screen
	FramebufferInitialize();
	
	// clean screen
	clear();
	
	//KEYBOARD
	if (!USPiEnvInitialize ())
	{
		return -2;
	}
	
	if (!USPiInitialize ())
	{

		USPiEnvClose ();

		return -2;
	}
	
	if (!USPiKeyboardAvailable ())
	{

		USPiEnvClose ();

		return -2;
	}

	USPiKeyboardRegisterKeyPressedHandler(KeyPressedHandler);


	// just wait and turn the rotor
	//~ for (unsigned nCount = 0; 1; nCount++)
	//~ {
		//~ ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
	//~ }
	//KEYBOARD
	
	
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
