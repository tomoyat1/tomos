/*
   tables.h
*/

#ifndef _TABLES_H_
#define _TABLES_H_

#include <stdint.h>

typedef struct
{
	uint16_t segment_limit_low;	
	uint16_t base_address_low;
	uint8_t base_address_mid;
	uint16_t bytes_eight_to_twentythree;
	uint8_t base_address_high;	
} segdesc_t __attribute__ ((packed));

typedef uint16_t segselect_t;

segdesc_t create_segdesc(unsigned int seglim, unsigned int baseaddr, uint16_t type, uint16_t systemf, uint16_t dpl, uint16_t presentf, uint16_t d_bf,uint16_t granularityf);
//TODO: Remember to set bits 20 and 21 to zero. (AVL and L flags respectively. Also, consider formating bits 8 to 23 in a seperate routine. The blob above is hard to read.


segselect_t create_segselect(uint8_t rpl, uint8_t ti, uint16_t index);

void set_idt();
void set_gdt();

#endif
