#include "src/syscall.h"

void dummy()
{
	return;
}

int div(int dividend, int divisor)
{
	int result = 0;
	int remainder = dividend;
	while (remainder >= divisor) {
		result++;
		remainder -= divisor;
	}
	return result;
}

int compute_volume(int rad)
{
	int rad3 = rad * rad * rad;
	return div(4*355*rad3, 3*113);
}

void kmain( void )
{
	//__asm("mrs r0, spsr");
	__asm("cps 0x10");
	while(1)
	{
		sys_nop();
	}
	sys_reboot();
}
