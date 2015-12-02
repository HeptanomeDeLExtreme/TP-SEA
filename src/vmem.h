
#include "banane.h"

int PAGE_SIZE = 4;
int SECON_LVL_TT_COUN = 8;
int SECON_LVL_TT_SIZE = 1024;
int FIRST_LVL_TT_COUN = 5;
int FIRST_LVL_TT_SIZE = 16384;


uint32_t first_table_flags = 0000000001;

uint32_t device_flags = 010000110110;

uint32_t normal_flags = 010001110010;

void start_mmu_C();

void configure_mmu_C();

unsigned int init_kern_translation_table(void);


