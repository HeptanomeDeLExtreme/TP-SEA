struct pcb_s{
uint32_t reg0;
uint32_t reg1;
uint32_t reg2;
uint32_t reg3;
uint32_t reg4;
uint32_t reg5;
uint32_t reg6;
uint32_t reg7;
uint32_t reg8;
uint32_t reg9;
uint32_t reg10;
uint32_t reg11;
uint32_t reg12;
uint32_t reg13;
uint32_t reg14;
uint32_t regPC;
uint32_t regCPSR;
}

struct pcb_s *current_process;

struct pcb_s kmain_process;
kmain_process.reg0;
kmain_process.reg1;
kmain_process.reg2;
kmain_process.reg3;
kmain_process.reg4;
kmain_process.reg5;
kmain_process.reg6;
kmain_process.reg7;
kmain_process.reg8;
kmain_process.reg9;
kmain_process.reg10;
kmain_process.reg11;
kmain_process.reg12;
kmain_process.reg13;
kmain_process.reg14;
kmain_process.regPC;
kmain_process.regCPSR;

void sys_yieldto(struct pcb_s* dest);

void do_sys_yieldto();
