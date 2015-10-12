#include "stdint.h"

struct pcb_s{
uint32_t reg[13];
uint32_t lr_svc;
uint32_t lr_user;
uint32_t cpsr;
};

struct pcb_s *current_process;

struct pcb_s kmain_process;


void sys_yieldto(struct pcb_s* dest);

void do_sys_yieldto();

void sched_init();
