/*************************************************************************
                           SYSCALL  -  description
                             -------------------
    début                : SYSCALL
    copyright            : (C) SYSCALL par SYSCALL
    e-mail               : SYSCALL
*************************************************************************/

//---------- Réalisation du module <SYSCALL> (fichier syscall.c) -----

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "syscall.h"
#include "util.h"
#include "hw.h"
#include <stdint.h>
#include "sched.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes
#define SYS_REBOOT  1
#define SYS_NOP  2
#define SYS_SETTIME 3
#define SYS_GETTIME 4

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

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
int stackPointer;
int call;


void do_sys_reboot();
void do_sys_nop();
void do_sys_settime(int stackPointer);
void do_sys_gettime(int stackPointer);
void pouet();


void __attribute__((naked)) swi_handler()
{

     __asm("stmfd sp!, {r0-r12,lr}");
     __asm("mov %0, sp":"=r"(stackPointer));

    __asm("mov %0, r0":"=r"(call)); //retrieve the number of the system call

    switch (call) {

    case SYS_REBOOT :
        do_sys_reboot();

        break;

    case SYS_NOP :
        do_sys_nop();

        break;
    case SYS_SETTIME :
        do_sys_settime(stackPointer);
        break;
    case SYS_GETTIME :
        do_sys_gettime(stackPointer);
        break;
    case SYS_YIELDTO :
        do_sys_yieldto(stackPointer);
        break;
    case SYS_YIELD :
        do_sys_yield(stackPointer);
        break;
    case SYS_EXIT :
        do_sys_exit(stackPointer);
        break;
    default:

        PANIC();

    }
    __asm("ldmfd sp!, {r0-r12,pc}^");

}

void sys_settime (uint64_t date_ms)
{


    pouet();
    int call = SYS_SETTIME;

    __asm("mov r0, %0"::"r"(call));


    __asm("mov r2, %0"::"r"(date_ms)); //first argument

    __asm("SWI #0"); //interrupt mode

    return;

}


uint64_t sys_gettime ()
{


    pouet();
    int call = SYS_GETTIME;
    uint32_t low_bits ;
    uint32_t high_bits ;


    __asm("mov r0, %0"::"r"(call));

    __asm("SWI #0"); //interrupt mode



    __asm("mov %0, r0":"=r"(low_bits)::"r0","r1");
    __asm("mov %0, r1":"=r"(high_bits));


    uint64_t date_ms = ((uint64_t)high_bits)<< 32 | low_bits;

    return date_ms;

}


void sys_reboot ()
// Algorithme :
//
{
    int call = SYS_REBOOT;

    __asm("mov r0, %0"::"r"(call));

    __asm("SWI #0"); //interrupt mode



} //----- fin de Nom


void pouet()
{

}

void sys_nop()
{

    pouet();
    int call = SYS_NOP;
    __asm("mov r0, %0"::"r"(call));


    __asm("SWI #0"); //interrupt mode

    return;

}

void do_sys_reboot()
{

    __asm("mov pc, #0x0000");
    return;

}

void do_sys_nop()
{



    return;

}

void do_sys_settime(int stackPointer)
{



    uint32_t low_bits = 0;
    uint32_t high_bits = 0;

    //first we need to reconstruct the int.
     __asm("mov r1, %0"::"r"(stackPointer)); //first argument
     __asm("ldr %0, [r1, #4]":"=r"(high_bits)); //first argument is at sp+4
     __asm("mov r1, %0"::"r"(stackPointer)); //first argument
     __asm("ldr %0, [r1, #8]":"=r"(low_bits)); //argument on 64bits
    uint64_t date_ms = ((uint64_t)high_bits)<< 32 | low_bits;

    set_date_ms(date_ms);

    return;

}


void do_sys_gettime(int stackPointer)
{


    uint64_t ret = get_date_ms();



     __asm("mov r1, %0"::"r"(stackPointer)); //first argument
     __asm("strd  %0, [r1]"::"r"(ret)); //first argument

    return;

}



