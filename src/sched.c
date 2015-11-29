/*************************************************************************
                           SCHED  -  description
                             -------------------
    début                : SCHED
    copyright            : (C) SCHED par SCHED
    e-mail               : SCHED
*************************************************************************/

//---------- Réalisation du module <SCHED> (fichier sched.c) -----

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "sched.h"
#include "kheap.h"
#include "hw.h"
#include "asm_tools.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types



//---------------------------------------------------- Variables statiques
struct pcb_s *current_process; //the current process
struct pcb_s kmain_process; //the main process, à initialiser
int stackPointer2;
int lr_irq;

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void pouetpouet();

void elect();

void irq_handler()
{



    __asm("mov %0, lr":"=r"(lr_irq));

    /** On passe en SVC **/
    __asm("cps 0x13"); // switch CPU to SVC mode

    /** On met les registres dans la pile **/
    __asm("push {%0}"::"r"(lr_irq-4));
    __asm("stmfd sp!, {r0-r12}");

    __asm("mov %0, sp":"=r"(stackPointer2));

    /** Changement de contexte **/

    do_sys_yield(stackPointer2);


    /** On restaure les registres **/
    __asm("ldmfd sp!, {r0-r12}");
    __asm("pop {%0}":"=r"(lr_irq));



    set_next_tick_default();
    ENABLE_TIMER_IRQ();
    ENABLE_IRQ();


    __asm("cps 0x10"); // switch CPU to USER mode

    __asm("mov pc, %0"::"r"(lr_irq));


}

void sys_exit(int status)
{
    int call = SYS_EXIT;

    __asm("mov r0, %0"::"r"(call));
    __asm("mov r1, %0"::"r"(status));

    __asm("SWI #0"); //interrupt mode


}

void do_sys_exit(int stackPointer)
{
    struct pcb_s* zombie = current_process;

    int plus_de_process = 1;

    if(zombie->suivao!=zombie)
    {

        do_sys_yield(stackPointer);

        zombie->precedao->suivao = zombie->suivao;
        zombie->suivao->precedao = zombie->precedao;
        /** Et pis c'est tout **/

        plus_de_process = 0;

    }

    /** On free la stack, et le PCB t'as vu? **/
    kFree(zombie->debut_pile, 10*1024);
    kFree((uint8_t*)zombie, sizeof(struct pcb_s));

    if(plus_de_process)
    {
        terminate_kernel();
    }

    /** WESH ON STOCKE OU LE CODE DE RETOUR SI ON LIBERE LE PCB??? **/
}

void sys_yield()
{
    int call = SYS_YIELD;

    __asm("mov r0, %0"::"r"(call));


    __asm("SWI #0"); //interrupt mode
}




void do_sys_yield(int stackPointer)
{
    /** On récupère les registres du processus courant dans la pile et on les range dans la struct **/

        int i = 0;

        for (i = 0; i < 13; i++)
           *((uint32_t*) current_process + i) = *((uint32_t*) stackPointer + i);
        current_process->lr_svc = *((uint32_t*)stackPointer + 13);

        /** On enregistre le cpsr (enregistré dans spsr quand changement de mode) **/
        __asm("mrs %0, spsr":"=r"(current_process->cpsr_user));

        __asm("cps 0x1F"); // switch CPU to SYSTEM mode

        /** On récupère l'adresse de retour du processus et son pointeur de pile (dynamique) **/
        __asm("mov %0, lr":"=r"(current_process->lr_user)); //
        __asm("mov %0, sp":"=r"(current_process->sp)); //

        __asm("cps 0x13"); // switch CPU to SVC mode

        /** Changement de processus **/
        elect();


        __asm("cps 0x1F"); // switch CPU to SYSTEM mode

        /** Récupère l'adresse de retour après le sys_yieldto **/
        __asm("mov lr, %0"::"r"(current_process->lr_user)); // we s
        __asm("mov sp, %0"::"r"(current_process->sp)); // we s
        //__asm("msr cpsr, %0"::"r"(current_process->cpsr_user));

        __asm("cps 0x13"); // switch CPU to SVC mode


        /** Récupération de tout le bouzin **/
        __asm("msr spsr, %0"::"r"(current_process->cpsr_user));

        for (i = 0; i < 13; i++)
           *((uint32_t*) stackPointer + i) = *((uint32_t*) current_process + i);
        *((uint32_t*) stackPointer + 13) = current_process->lr_svc;

}

void sched_init()
{

    ////////** Initialisaçao de la liste **///////
    /** Bijour, ci moi la banane qui code :) **/
    kmain_process.precedao = &kmain_process;
    kmain_process.suivao = &kmain_process;

    current_process = &kmain_process;
    kheap_init();
}


void create_process(func_t* entry)
{



    struct pcb_s* result = (struct pcb_s*) kAlloc(sizeof(struct pcb_s));
    void* stack = kAlloc(10*1024);


    result->sp = stack + 2560; //beginning of the stack (empty)
    result->lr_svc=(int)entry;
    result->cpsr_user=0x150; //1 0 1 0 10000

    result->debut_pile = stack;
    /** On met le nouveau processao dans la liste **/

    result->precedao = current_process->precedao;
    result->suivao = current_process;

    current_process->precedao->suivao = result;
    current_process->precedao = result;


    return;

}

void elect()
{
    current_process = current_process->suivao;
}





void pouetpouet(){}

/** ATTENTION, CODE BULLSHIT (PAS) SALE DIGNE D'UN CP
I quote, Mr Matrix :""Bah il est pas sale èèh! Si on enléve les commentééres l'é po sale èèh!**/

void /**__attribute__((naked))*/ sys_yieldto(struct pcb_s* dest)
{

   // __asm("mov %0, lr":"=r"(current_process->lr_user));
   // __asm("mov %0, sp":"=r"(current_process->sp));
    //__asm("mrs %0, cpsr":"=r"(current_process->cpsr_user));
   // pouetpouet();
    int call = SYS_YIELDTO;



    __asm("mov r0, %0"::"r"(call));


    __asm("mov r1, %0"::"r"(dest));//first argument



    __asm("SWI #0"); //interrupt mode


  //  __asm("msr cpsr, %0"::"r"(current_process->cpsr_user));
   // __asm("mov sp, %0"::"r"(current_process->sp));
  //  __asm("blx %0"::"r"(current_process->lr_user));


    return;
}

void do_sys_yieldto(int stackPointer)
{

    //*current_process = *((struct pcb_s*)stackPointer); //recopie contexte dans current_process




/** On récupère les registres du processus courant dans la pile et on les range dans la struct **/

    __asm("mov r1, %0"::"r"(stackPointer)); //sp dans r1
    __asm("ldr %0, [r1]":"=r"(current_process->r0)); //argument on 64bits
    __asm("ldr %0, [r1, #4]":"=r"(current_process->r1)); //argument on 64bits
    __asm("ldr %0, [r1, #8]":"=r"(current_process->r2)); //argument on 64bits
    __asm("ldr %0, [r1, #12]":"=r"(current_process->r3)); //argument on 64bits
    __asm("ldr %0, [r1, #16]":"=r"(current_process->r4)); //argument on 64bits
    __asm("ldr %0, [r1, #20]":"=r"(current_process->r5)); //argument on 64bits
    __asm("ldr %0, [r1, #24]":"=r"(current_process->r6)); //argument on 64bits
    __asm("ldr %0, [r1, #28]":"=r"(current_process->r7)); //argument on 64bits
    __asm("ldr %0, [r1, #32]":"=r"(current_process->r8)); //argument on 64bits
    __asm("ldr %0, [r1, #36]":"=r"(current_process->r9)); //argument on 64bits
    __asm("ldr %0, [r1, #40]":"=r"(current_process->r10)); //argument on 64bits
    __asm("ldr %0, [r1, #44]":"=r"(current_process->r11)); //argument on 64bits
    __asm("ldr %0, [r1, #48]":"=r"(current_process->r12)); //argument on 64bits
    //__asm("ldr %0, [r1, #52]":"=r"(current_process->sp)); //argument on 64bits
    __asm("ldr %0, [r1, #52]":"=r"(current_process->lr_svc)); //argument on 64bits


    /** On enregistre le cpsr (enregistré dans spsr quand changement de mode) **/
    __asm("mrs %0, spsr":"=r"(current_process->cpsr_user));



    __asm("cps 0x1F"); // switch CPU to SYSTEM mode


    /** On récupère l'adresse de retour du processus et son pointeur de pile (dynamique) **/
    __asm("mov %0, lr":"=r"(current_process->lr_user)); //
    __asm("mov %0, sp":"=r"(current_process->sp)); //


    __asm("cps 0x13"); // switch CPU to SVC mode


    /** BULLSHIT **/
    int lr_svc = current_process->lr_svc;

    /** Changement de processus **/
    __asm("mov r1, %0"::"r"(stackPointer)); //sp dans r1
    __asm("ldr %0, [r1, #4]":"=r"(current_process));//current_process vers nouveau processus



    __asm("cps 0x1F"); // switch CPU to SYSTEM mode


    /** Récupère l'adresse de retour après le sys_yieldto **/
    __asm("mov lr, %0"::"r"(current_process->lr_user)); // we s
    __asm("mov sp, %0"::"r"(current_process->sp)); // we s
    //__asm("msr cpsr, %0"::"r"(current_process->cpsr_user));

    __asm("cps 0x13"); // switch CPU to SVC mode


    /** Récupération de tout le bouzin **/
    __asm("msr spsr, %0"::"r"(current_process->cpsr_user));

    __asm("mov r1, %0"::"r"(stackPointer)); //sp dans r1
    __asm("str %0, [r1]"::"r"(current_process->r0)); //écrire nouveau contexte
    __asm("str %0, [r1, #4]"::"r"(current_process->r1)); //écrire nouveau contexte
    __asm("str %0, [r1, #8]"::"r"(current_process->r2)); //écrire nouveau contexte
    __asm("str %0, [r1, #12]"::"r"(current_process->r3)); //écrire nouveau contexte
    __asm("str %0, [r1, #16]"::"r"(current_process->r4)); //écrire nouveau contexte
    __asm("str %0, [r1, #20]"::"r"(current_process->r5)); //écrire nouveau contexte
    __asm("str %0, [r1, #24]"::"r"(current_process->r6)); //écrire nouveau contexte
    __asm("str %0, [r1, #28]"::"r"(current_process->r7)); //écrire nouveau contexte
    __asm("str %0, [r1, #32]"::"r"(current_process->r8)); //écrire nouveau contexte
    __asm("str %0, [r1, #36]"::"r"(current_process->r9)); //écrire nouveau contexte
    __asm("str %0, [r1, #40]"::"r"(current_process->r10)); //écrire nouveau contexte
    __asm("str %0, [r1, #44]"::"r"(current_process->r11)); //écrire nouveau contexte
    __asm("str %0, [r1, #48]"::"r"(current_process->r12)); //écrire nouveau contexte
   // __asm("str %0, [r1, #52]"::"r"(current_process->sp)); //écrire nouveau contexte
    __asm("str %0, [r1, #52]"::"r"(lr_svc)); //écrire nouveau contexte lr_user






    return;
}
