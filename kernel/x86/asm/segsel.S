.code32
.section .text
.global segsel

segsel:
	pushl %ebp
	movl %esp, %ebp
	movl $0x10, %eax
	movl %eax, %ds 
	movl %eax, %es 
	movl %eax, %fs 
	movl %eax, %gs 
	movl %eax, %ss 
	ljmpl $0x08, $reload_CS
reload_CS: 
	pop %ebp
	ret

