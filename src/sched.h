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
#include "kheap.h"
#include "hw.h"
#include "asm_tools.h"
#include "banane.h"
#include <stdint.h>
#define SYS_YIELDTO 5
#define SYS_YIELD 6
#define SYS_EXIT 7

//------------------------------------------------------------------ Types

typedef int (func_t) (void);

struct pcb_s *current_process; //the current process
struct pcb_s kmain_process; //the main process, à initialiser

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


