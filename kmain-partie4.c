#include "util.h"
#include "syscall.h"
#include "stdint.h"

int
kmain( void )
{
	// Note: kmain() starts with cpu in SYSTEM mode

	__asm("cps 0x10"); // switch CPU to USER mode

	// **********************************************************************

	// Userland starts here
	uint64_t date_ms = 42;
	sys_settime(date_ms);
	sys_nop();


	// this must be reachable
	sys_reboot();

	return 0;
}
