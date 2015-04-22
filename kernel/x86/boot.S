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

.set KERNEL_VIRT_BASE, 0xC0000000
.set KERNEL_PAGE_INDEX, KERNEL_VIRT_BASE>>22
/*index of the 4 MiB page in which the kernel resides*/

.section .data
.align 0x1000
//map phys first page to virt first page after that, map physical first page to virtual page that starts at 0xC0000000
BootPageDirectory:
	.long 0x00000083
	.skip (KERNEL_PAGE_INDEX-1)*4	
	.long 0x00000083
	.skip 1024-KERNEL_PAGE_INDEX-1
//TODO:Once userland init starts, system should use that process's page directory. The page directory of every process should have the kernel mapped to 0xC000000 and above.

.section .text
.set loader, _loader-KERNEL_VIRT_BASE
.global loader

.type _loader, @function
.global _loader
_loader:
/*Enable paging!!*/
	movl $BootPageDirectory-KERNEL_VIRT_BASE, %ecx
	movl %ecx, %cr3
	movl %cr4, %ecx
	orl $0x00000010, %ecx
	movl %ecx, %cr4
	movl %cr0, %ecx
	orl $0x80000000, %ecx
	movl %ecx, %cr0
	leal BootPageDirectory, %ecx
	jmp *%ecx

_higher_kernel:
	movl $KERNEL_VIRT_BASE+0x00100000,%ecx
	jmp *%ecx 
	call _start_kernel
	/*Shouldn't return*/
	cli
	hlt
.Lhang:
	jmp .Lhang

.size _loader, . - _loader

