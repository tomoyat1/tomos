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
//#include <kernel/kprint.h>

void multiboot_start()
{
  //kprint("Booting")

  //hocus-pocus to relocate kernel at high memory.

  //GDT

  //IDT


}
