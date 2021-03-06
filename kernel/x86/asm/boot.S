.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.set KERNEL_VIRT_BASE, 0xC0000000
.set KERNEL_PAGE_INDEX, KERNEL_VIRT_BASE>>22
/*index of the 4 MiB page in which the kernel resides*/

.section .MultibootHeader
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .data
.align 0x1000
/*
 * Map phys first page to virt first page after that,
 * map physical first page to virtual page that starts at 0xC0000000
 * Map memory mapped registers for local and io APICs before
 * KERNEL_VIRT_BASE
 */
.global kernelpagedir
kernelpagedir:
	.long 0x00000083
	.skip (KERNEL_PAGE_INDEX-1-1)*4
	.long 0xFEC0009B
	.long 0x00000083
	.long 0x00400083
	.long 0x00800083
	.skip (1024-KERNEL_PAGE_INDEX-3)*4
//TODO:Once userland init starts, system should use that process's page directory. The page directory of every process should have the kernel mapped to 0xC000000 and above.

.section .text

.type _loader, @function
.global _loader

_loader:
/*Enable paging!!*/
	movl $kernelpagedir-KERNEL_VIRT_BASE, %ecx
	movl %ecx, %cr3

	movl %cr4, %ecx
	orl $0x00000010, %ecx
	movl %ecx, %cr4

	movl %cr0, %ecx
	orl $0x80000000, %ecx
	movl %ecx, %cr0

	leal higher_kernel, %ecx
	jmp *%ecx

higher_kernel:
	movl $0, kernelpagedir	
	invlpg 0
	movl $kernel_stack_bottom, %esp
	movl $kernel_stack_bottom, %ebp
	pushl %eax
	addl $KERNEL_VIRT_BASE, %ebx //make mb info struct address linear.
	pushl %ebx
	call start_kernel
	/*Shouldn't return*/
	cli
	hlt
.Lhang:
	jmp .Lhang

.size _loader, . - _loader

.section .bss
.align 0x1000
kernel_stack_top:
.skip 0x00001000
.global kernel_stack_bottom
kernel_stack_bottom:
.skip 0x8

.global kernel_heap
.align 0x1000
kernel_heap:
