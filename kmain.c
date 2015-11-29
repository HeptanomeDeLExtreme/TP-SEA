
#include "syscall.h"
#include "util.h"
#include "sched.h"
#include "hw.h"
#include "asm_tools.h"


#define NB_PROCESS 5

void dummy()
{
	return;
}

int div(int dividend, int divisor)
{
	int result = 0;
	int remainder = dividend;
	
	while(remainder >= divisor) 
	{
		result++;
		remainder -= divisor;
	}

    return result;
}

int compute_volume(int rad)
{
	int rad3 = rad * rad * rad;

	return div(4*355*rad3, 3*113);	// V = (4/3)*pi*R³
}



struct pcb_s pcb1, pcb2; //contextes de p1, p2

struct pcb_s *p1, *p2; //pointeurs vers pcb1, pcb2

void user_process_1()
{

    int v1=5;
    int v2=5;
    int v3=5;
    int v4=5;
    int v5=5;
    int v6=5;
    int v7=5;
    int v8=5;
    int v9=5;
    int v10=5;
    while(1)
    {


        v1++;
        v2++;
        v3++;
        v4++;
        v5++;
        v6++;
        v7++;
        v8++;
        v9++;
        v10++;
        //sys_yieldto(p2);

    }


}

void user_process_2()
{

    int v2 = -12;
    while(1)
    {
        v2-=2;
        //sys_yieldto(p1);

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

void user_process()
{
    int v=0;
    while(v<5)
    {
        v++;
        sys_yield();
    }

    sys_exit(0);
}



int kmain (void)
{
    /**__asm("cps #16");
    __asm("nop");
    __asm("cps #19");
    //__asm("bl dummy");
	int radius = 5;
    __asm("mov %0, r2":"=r"(radius));
    __asm("mov r3, %0":"=r"(radius));*/

    sched_init(); //pour mettre current_process vers le contexte de kmain

    /**p1=&pcb1; //évident
    p2=&pcb2;

    p1->lr_user = (int) &user_process_1; //pour partir au début des fonctions aux lancements des processus
    p2->lr_user = (int) &user_process_2;
<<<<<<< HEAD
    p1->r4 = 0x4567;
=======
    p1->r4 = 0x4567;*/
/** ATTENTION, commentaire dégueulasse ci-dessous:
    p1=create_process((func_t*) &user_process_1);
    p2=create_process((func_t*) &user_process_2);
**/
    /** ENCULEEEE **/
    create_process((func_t*)&user_process_1);
    create_process((func_t*)&user_process_2);
    create_process((func_t*)&user_process_3);

    timer_init();
    ENABLE_IRQ();


    /*int i;
    for(i=0;i<NB_PROCESS;i++)
    {
        create_process((func_t*)&user_process);

    }*/
    __asm("cps 0x10");



   //**//**//**// sys_yieldto(p1); //laisse la main à p1

    int i=0;

    while(i<100)
    {
        sys_yield();
        i++;
    }

    //**PANIC();
    /**int volume;

	dummy();
    volume = compute_volume(radius);*/

    sys_exit(0);


    /** Bonjour, je suis un return, mais jamais on ne m'exécute moi :( **/
    return 0;
}
