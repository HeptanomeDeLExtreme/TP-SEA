/*************************************************************************
                           SCHED  -  description
                             -------------------
    début                : SCHED
    copyright            : (C) SCHED par SCHED
    e-mail               : SCHED
*************************************************************************/

//---------- Interface du module <SCHED> (fichier sched.h) ---------
#if ! defined ( SCHED_H )
#define SCHED_H

//------------------------------------------------------------------------
// Rôle du module <SCHED>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

#define SYS_YIELDTO 5
#define SYS_YIELD 6
#define SYS_EXIT 7

//------------------------------------------------------------------ Types

struct pcb_s
{
    int r0;
    int r1;
    int r2;
    int r3;
    int r4;
    int r5;
    int r6;
    int r7;
    int r8;
    int r9;
    int r10;
    int r11;
    int r12;
    int lr_svc;
    int lr_user;
    void* sp;
    int pc;
    int cpsr_user;
    struct pcb_s* precedao;
    struct pcb_s* suivao;

    void* debut_pile;
    int code_retour;
};

typedef int (func_t) (void);

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void sys_yield();

void do_sys_exit(int stackPointer);

void do_sys_yield(int stackPointer);

void sys_yieldto(struct pcb_s* dest);

void do_sys_yieldto(int stackPointer);

void sched_init();

void sys_exit(int status);

void create_process(func_t* entry);



#endif // SCHED_H


