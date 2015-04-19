/*
 *  kernel.c
 *  Entrypoint for the kernel. multiboot_start() assumes being called from
 *  a Multiboot standard compliant bootloader. (ofc execution starts in 32
 *  bit protected mode.
 *
 *
 */

#include <kernel/tables.h>
//#include <kernel/vga.h>

void multiboot_start()
{
  //kprint("Booting")

  //IDT

  //GDT

  //LDT

}
