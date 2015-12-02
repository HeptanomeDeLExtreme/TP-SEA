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

unsigned int
init_kern_translation_table(void){
	uint8_t* first_level_table = kAlloc_aligned(FIRST_LVL_TT_SIZE, 14);
	MMUTABLEBASE = (unsigned int)first_level_table;
	for (uint32_t i = 0; i< FIRST_LVL_TT_SIZE; i++){
		uint8_t* second_level_table = kAlloc_aligned(SECON_LVL_TT_SIZE, 10);
		first_level_table[i] = (uint32_t)first_table_flags | ((uint32_t)second_level_table & 0xFFFFFC00);
		for(uint32_t j=0; j<SECON_LVL_TT_SIZE ; j++){
			if(j<__kernel_heap_end__){
				second_level_table[j] = device_flags | (j<<12);
			}
			else if(j>0x20000000 && j<0x20FFFFFF){
				second_level_table[j] = device_flags | (j<<12);
			}
			else if(j>__kernel_heap_end__) {
				second_level_table[j] = normal_flags | (j<<12);
			}
			else{
				second_level_table[j] =0;
			}
		}
		first_level_table ++;
	}
	return 0;
}

void vmem_init(){
	init_kern_translation_table();
	configure_mmu_C();
	uint32_t cpsr = 0;
	__asm("mrs r0, CPSR");
	__asm("mov %0, r0":"=r"(cpsr));
	cpsr+=0xC0;
	__asm("msr cpsr, %0"::"r"(cpsr));
	start_mmu_C();

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
