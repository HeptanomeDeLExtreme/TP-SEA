#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"


void user_process_1()
{
    int v1=5;
    while(1)
    {
        v1++;
    }


}

void user_process_2()
{
    int v2 = -12;
    while(1)
    {
        v2-=2;
    }
}

void user_process_3()
{
    int v3 = 0;
    while(1)
    {
        v3+=5;

    }
}

int kmain (void)
{

    sched_init(); 

    create_process((func_t*)&user_process_1);
    create_process((func_t*)&user_process_2);
    create_process((func_t*)&user_process_3);

    timer_init();
    ENABLE_IRQ();

    __asm("cps 0x10");

    while(1)
    {
        sys_yield();
    }

    return 0;
}
