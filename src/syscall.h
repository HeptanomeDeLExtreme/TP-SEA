#include "stdint.h"

uint32_t* top_of_stack;
enum numAppel{REBOOT,NOP,SETTIME,GETTIME,YIELDTO};

void sys_reboot();

void swi_handler();

void do_sys_reboot();

void sys_nop();

void do_sys_nop();

void sys_settime(uint64_t date_ms);

void do_sys_settime();

uint64_t sys_gettime();

void do_sys_gettime();
