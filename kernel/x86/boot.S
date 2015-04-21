global _loader
extern _multiboot_start

.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN|MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.set KERNEL_LOAD_ADDR, 0xC1000000
.set KERNEL_PAGE_OFFSET, 0xC0000000<<22

.section .data
.align 0x1000
/*map phys first page to virt first page
 * after that, map phys first page to virt page that starts at 0xC0000000
 */

BootPageDirectory:
	.long 0x00000083
	.skip KERNEL_PAGE_OFFSET*4	
	.long 0x00000083
	.skip 1024-KERNEL_PAGE_OFFSET-1

.section .text

.set loader, _loader-0xC0000000
global loader

.type _loader, @function
_loader:

	cli
	hlt
.Lhang:
	jmp .Lhang

.size _loader, . - _loader

