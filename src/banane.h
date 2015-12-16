uint32_t MMUTABLEBASE;
uint32_t OCCUPATION_TABLE;

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
	uint32_t page_table;
	uint32_t occupation_table;

    void* debut_pile;
    int code_retour;
};

void vmem_init();

uint32_t vmem_translate(uint32_t va, struct pcb_s* process); 

uint8_t* vmem_alloc_for_userland(struct pcb_s* process, unsigned int size);
