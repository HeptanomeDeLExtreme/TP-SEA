
#include "banane.h"

const int PAGE_SIZE = 4;
const int SECON_LVL_TT_COUN = 256;
const int SECON_LVL_TT_SIZE = 1024;
const int FIRST_LVL_TT_COUN = 4096;
const int FIRST_LVL_TT_SIZE = 16384;
const int TAILLE = 0x20FFFFFF;
const int PAGE_NUMBER = 0x8400000;

// First translation table flags
uint32_t first_table_flags =
  (0 << 9) | // P = 0
  (0 << 5) | // Domain = 0
  (0 << 4) | // SBZ = 0
  (0 << 3) | // NS = 0
  (0 << 2) | // SBZ = 0
  (0 << 1) | // Always 0
  (1 << 0); // Always 1

// Second TT device part flags
uint32_t device_flags =
  (0 << 11) | // nG = 0
  (1 << 10) | // S = 1
  (0 << 9) | // APX = 0
  (000 << 6) | // TEX = 000
  (11 << 4) | // AP = 11
  (0 << 3) | // C = 0
  (1 << 2) | // B = 1
  (1 << 1) | // Always 1
  (0 << 0); // XN = 0

// Second TT default flags
uint32_t normal_flags =
  (0 << 11) | // nG = 0
  (0 << 10) | // S = 0
  (0 << 9) | // APX = 0
  (001 << 6) | // TEX = 001
  (00 << 4) | // AP = 00
  (0 << 3) | // C = 0
  (0 << 2) | // B = 0
  (1 << 1) | // Always 1
  (0 << 0); // XN = 0

void start_mmu_C();

void configure_mmu_C();

unsigned int init_kern_translation_table(void);

uint32_t 
find_n_free_pages(struct pcb_s* process, unsigned int nb_pages);

uint32_t
find_one_free_frame(struct pcb_s* process);

uint32_t
vmem_translate(uint32_t va, struct pcb_s* process);

void
add_translation(uint32_t va, uint32_t pa);

void vmem_init();
