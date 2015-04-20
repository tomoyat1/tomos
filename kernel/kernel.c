/*
 *  kernel.c
 *  Entrypoint for the kernel. multiboot_start() assumes being called from
 *  a Multiboot standard compliant bootloader. (ofc execution starts in 32
 *  bit protected mode.
 *
 *
 */

#include <kernel/tables.h>
#include <kernel/pages.h>
//#include <kernel/vga.h>
//#include <kernel/kprint.h>

void multiboot_start()
{
  //kprint("Booting")

  //TODO: write the linker script so that the kernel gets loaded at the 3GiBmark. Perhaps in asm??
  //enable paging
  //GDT

  //IDT


}
