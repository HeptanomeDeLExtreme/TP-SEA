#include "src/syscall.h"
#include "stdint.h"

void kmain( void )
{
	//__asm("mrs r0, spsr");
	__asm("cps 0x10");
	//sys_settime(18446744073709551613);
	sys_settime(16);
	uint64_t actual = sys_gettime();
	actual++;
}
