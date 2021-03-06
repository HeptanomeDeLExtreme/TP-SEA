uint32_t MMUTABLEBASE;

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

    void* debut_pile;
    int code_retour;
};

unsigned int init_kern_translation_table(void);

void vmem_init();

uint32_t vmem_translate(uint32_t va, struct pcb_s* process); 

uint8_t* vmem_alloc_for_userland(struct pcb_s* process, unsigned int size);

void vmem_free(uint8_t* vAddress, struct pcb_s* process, unsigned int size);
