#include "src/syscall.h"
#include "stdint.h"

void kmain( void )
{
	//__asm("mrs r0, spsr");
	__asm("cps 0x10");
	//sys_settime(18446744073709551613);
	uint64_t heure = 16;
	sys_settime(heure);
	uint64_t actual = sys_gettime();
	actual++;
}
