#include "util.h"
#include "syscall.h"

int
kmain( void )
{
	// Note: kmain() starts with cpu in SYSTEM mode

	__asm("cps 0x10"); // switch CPU to USER mode

	// **********************************************************************

	// Userland starts here
	while(1){
		sys_nop();
	}

	// this must be reachable
	sys_reboot();

	return 0;
}
