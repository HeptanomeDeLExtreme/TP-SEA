#include "asm_tools.h"
#include "vmem.h"
#include "kheap.h"
#include "stdlib.h"


void
start_mmu_C()
{
	register unsigned int control;
	__asm("mcr p15, 0, %[zero], c1, c0, 0" : : [zero] "r"(0)); //Disable cache
	__asm("mcr p15, 0, r0, c7, c7, 0"); //Invalidate cache (data and instructions) */
	__asm("mcr p15, 0, r0, c8, c7, 0"); //Invalidate TLB entries
	/* Enable ARMv6 MMU features (disable sub-page AP) */
	control = (1<<23) | (1 << 15) | (1 << 4) | 1;
	/* Invalidate the translation lookaside buffer (TLB) */
	__asm volatile("mcr p15, 0, %[data], c8, c7, 0" : : [data] "r" (0));
	/* Write control register */
	__asm volatile("mcr p15, 0, %[control], c1, c0, 0" : : [control] "r" (control));
}

void
configure_mmu_C()
{
	register unsigned int pt_addr = MMUTABLEBASE;
	//total++;
	/* Translation table 0 */
	__asm volatile("mcr p15, 0, %[addr], c2, c0, 0" : : [addr] "r" (pt_addr));
	/* Translation table 1 */
	__asm volatile("mcr p15, 0, %[addr], c2, c0, 1" : : [addr] "r" (pt_addr));
	/* Use translation table 0 for everything */
	__asm volatile("mcr p15, 0, %[n], c2, c0, 2" : : [n] "r" (0));
	/* Set Domain 0 ACL to "Manager", not enforcing memory permissions
	* Every mapped section/page is in domain 0
	*/
	__asm volatile("mcr p15, 0, %[r], c3, c0, 0" : : [r] "r" (0x3));
}


void
add_translation(uint32_t va, uint32_t pa) // virtual/physical adress
{
    uint32_t first_level_entry = (va >> 20);
    uint32_t* first_level_descriptor_address = (uint32_t*) (MMUTABLEBASE | (first_level_entry << 2));
    uint32_t first_level_descriptor = *(first_level_descriptor_address);
    
    static uint32_t* second_level_table = NULL; 

    // Valid if 01 (else : not allocated, we need to allocate)
    int valid_entry = 1;
    if (! (first_level_descriptor & 0x3)) {
	valid_entry = 0;
    }    

    if(! valid_entry) {
	second_level_table = (uint32_t*)kAlloc_aligned(SECON_LVL_TT_SIZE, 10);
    } else {
	uint32_t second_level_entry = ((va<<12)>>24);
        uint32_t second_level_place = first_level_descriptor & 0xFFFFFC00; 
	second_level_table = (uint32_t*) (second_level_place | (second_level_entry << 2));
    }
    
    /* On remplit la table de niv 2, qui est pointée par moult références interposées */
    *(second_level_table) = device_flags | ((uint32_t)((va>>12)<<12));
    *(first_level_descriptor_address) = (uint32_t)first_table_flags | ((uint32_t)second_level_table & 0xFFFFFC00);

}


unsigned int
init_kern_translation_table(void){
	uint32_t* first_level_table = (uint32_t*)kAlloc_aligned(FIRST_LVL_TT_SIZE, 14);
	MMUTABLEBASE = (unsigned int) first_level_table;

	//parcours de la table de niv 1
	for (uint32_t addr = 0 ; addr <= 0x20FFFFFF ; addr += PAGE_SIZE) {
		if(addr<(uint32_t)kernel_heap_limit) {
			add_translation(addr, addr); 			
		}
		else if(addr>0x20000000 && addr<0x20FFFFFF) {
			add_translation(addr, addr); 			
		}
	}
	return 0;
}


void vmem_init(){
	kheap_init();
	init_kern_translation_table();
	configure_mmu_C();
	uint32_t cpsr = 0;
	__asm("mrs r0, CPSR");
	__asm("mov %0, r0":"=r"(cpsr));
	cpsr+=0xC0;
	__asm("msr cpsr, %0"::"r"(cpsr));
	static unsigned int OCCUPATION_TABLE;
	init_occupation_table();
	start_mmu_C();

}

void
init_occupation_table(){
    uint8_t* occupation_table = NULL;
    occupation_table = (uint8_t*)kAlloc_aligned(PAGE_NUMBER);

    //parcours de la table de niv 1
    for (uint32_t addr = 0 ; addr <= 0x20FFFFFF ; addr += PAGE_SIZE) {
	if(addr<(uint32_t)kernel_heap_limit) {
	    *(occupation_table) = 1;			
	}

	else if(addr>0x20000000 && addr<0x20FFFFFF) {
	    *(occupation_table) = 1;
	}

	else {
	    *(occupation_table) = 0;
	}
	occupation_table++;
    }

    OCCUPATON_TABLE = (unsigned int) occupation_table;
}

uint8_t* vmem_alloc_for_userland(pcb_t* process, unsigned int size){
    
    /*
Table d'occupation des frames (physique) est remplie quand on remplit la TRANSLATION_TABLE
On veut, pour un process, allouer des adresses virtuelles contigues
Pour les frames allouées, ballec (c'est le but d'ailleurs)
Deux boucles :
-une sur le translation_table
-une le nombre de pages à allouer
But : trouver un coin de la translation_table assez grand pour mettre toutes les entrées du process
Impl : on veut NBR pages consécutives qui donnent translation_fault
=> Fonction 1 : find_n_free_pages

Ensuite, pour chaque entrée de translation_table (va), on cherche une frame libre
On va aussi boucler sur la table d'occupation et la màj si on alloue
Fonction 2 : find_one_free_frame

     */

    uint8_t occupation_table =  process->OCCUPATION_TABLE;

    nbr_pages_to_allocate = (uint32_t)(size/PAGE_SIZE) + 1;

    for (uint32_t addr = 0 ; addr <= 0x20FFFFFF ; addr += PAGE_SIZE) {
	int enough_space=1;
	// on cherche si il y a nb frames libres consécutives (on veut virt adresses contigues)
	// pue la merde
	for (uint32_t i = 0; i<nbr_pages_to_allocate ; i++){
	    if (*((uint8_t*)(occupation_table+i*(addr/4)))==1){
		enough_space = 0;
	    }
	}
	if(enough_space){
	    //Remplir mmu (vérif que phys adr est libre)
	    //Modifier table occupation
	}

	/* if(*((uint8_t*)(occupation_table+(addr/4)))==0) { */
	/*     //remplir la mmu en vérifiant l'utilisation de l'espace virtuel */
	/*     //modifier la table d'occupation */
	/*     break; */
	/* } */
    }

uint32_t
vmem_translate(uint32_t va, struct pcb_s* process)
{
	uint32_t pa; /* The result */

	/* 1st and 2nd table addresses */
	uint32_t table_base;
	uint32_t second_level_table;

	/* Indexes */
	uint32_t first_level_index;
	uint32_t second_level_index;
	uint32_t page_index;

	/* Descriptors */
	uint32_t first_level_descriptor;
	uint32_t* first_level_descriptor_address;
	uint32_t second_level_descriptor;
	uint32_t* second_level_descriptor_address;

	if (process == NULL)
	{
		__asm("mrc p15, 0, %[tb], c2, c0, 0" : [tb] "=r"(table_base));
	}

	else
	{
		table_base = (uint32_t) process->page_table;
	}

	table_base = table_base & 0xFFFFC000;

	/* Indexes*/
	first_level_index = (va >> 20);
	second_level_index = ((va << 12) >> 24);
	page_index = (va & 0x00000FFF);

	/* First level descriptor */
	first_level_descriptor_address = (uint32_t*) (table_base | (first_level_index << 2));
	first_level_descriptor = *(first_level_descriptor_address);

	/* Translation fault*/
	if (! (first_level_descriptor & 0x3)) {
		return (uint32_t) FORBIDDEN_ADDRESS;
	}

	/* Second level descriptor */
	second_level_table = first_level_descriptor & 0xFFFFFC00;
	second_level_descriptor_address = (uint32_t*) (second_level_table | (second_level_index << 2));
	second_level_descriptor = *((uint32_t*) second_level_descriptor_address);

	/* Translation fault*/
	if (! (second_level_descriptor & 0x3)) {
		return (uint32_t) FORBIDDEN_ADDRESS;
	}

	/* Physical address */
	pa = (second_level_descriptor & 0xFFFFF000) | page_index;

	return pa;
}
