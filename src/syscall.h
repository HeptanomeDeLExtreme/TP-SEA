/*************************************************************************
                           SYSCALL  -  description
                             -------------------
    début                : SYSCALL
    copyright            : (C) SYSCALL par SYSCALL
    e-mail               : SYSCALL
*************************************************************************/

//---------- Interface du module <SYSCALL> (fichier syscall.h) ---------
#if ! defined ( SYSCALL_H )
#define SYSCALL_H

//------------------------------------------------------------------------
// Rôle du module <SYSCALL>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include <stdint.h>
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
 void sys_reboot ();
// Mode d'emploi :
//
// Contrat :
//

 void sys_nop ();
// Mode d'emploi :
//
// Contrat :
//

 void sys_settime (uint64_t date_ms);
// Mode d'emploi :
//
// Contrat :
//

 uint64_t sys_gettime ();
// Mode d'emploi :
//
// Contrat :
//


#endif // SYSCALL_H


