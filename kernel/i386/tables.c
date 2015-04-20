/*
 *     tables.c
 *     IDT & GDT, LDT is obsolete
 *     File is name tables.c, but does more than that. Segment Descriptor stuff is 
 *     here as well.
 *
 */



#include <stddef.h>
#include <stdint.h>

#include <kernel/tables.h>


segdesc_t create_segdesc(unsigned int seglim, unsigned int baseaddr, uint16_t type, uint16_t systemf, uint16_t dpl, uint16_t presentf, uint16_t d_bf,uint16_t granularityf)
{
	segdesc_t new_descriptor;

	//declare what would become members.
	uint16_t seglim_low, baseaddr_low, b8to23;
	uint8_t baseaddr_mid, baseaddr_high; 

	//Split seglim into two
	seglim_low = (uint16_t) (seglim & 0x0000FFFF);
	uint16_t seglim_high = (uint16_t) ((seglim >> 16)) & 0x000F;

	//Split baseaddr into three	
	baseaddr_low = (uint16_t) seglim;
	baseaddr_mid = (uint8_t) (seglim >> 16);
	baseaddr_high = (uint8_t) (seglim >> 24);
	
	//flags
	uint16_t system_flag = systemf << 4;
	uint16_t present_flag = presentf << 7;
	uint16_t d_b_flag = d_bf << 14;
	uint16_t granularity_flag = granularityf << 15;
		
	//Other things in b8to23
	uint16_t dpl_shifted = dpl << 5;
		//type needs no shifting
	//bits 20 and 21	
	uint16_t bits2021 = 2 << 12;

	// put b8to23 together
	b8to23 = type & system_flag & dpl_shifted & present_flag & (seglim_high << 8) & bits2021 & d_b_flag & granularity_flag;	
	
	//write them out
	new_descriptor.segment_limit_low = seglim_low;
	new_descriptor.base_address_low = baseaddr_low;
	new_descriptor.base_address_mid = baseaddr_mid;
	new_descriptor.bytes_eight_to_twentythree = b8to23;
	new_descriptor.base_address_high = baseaddr_high;
	return new_descriptor;
}

segselect_t create_segselect(uint8_t rpl, uint8_t ti, uint16_t index)
{
	segselect_t new_selector;
	new_selector = rpl & ti << 2 & index << 3;
	return new_selector;
	
}
void set_idt()
{
	//Do stuff!!
}

void set_gdt()
{
	//Do stuff!!
	//KERNEL_TEXT, KERNEL_DATA,USER_TEXT,USER_DATA will all start from 0x00000000 and end at 0xFFFFFFFF
	// Screw segmentation. Paging is the way to go!
}
