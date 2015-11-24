//Entry points for irq handlers.

.global divided_by_zero
divided_by_zero:
	lcall $0x8, $divzero
	popl %edx
	iret

.global double_fault
	lcall $0x8, $double_fault_panic
	popl %edx
	iret

.global kbdirq
kbdirq:
	lcall $0x8, $keypress
	popl %edx
	iret

.global pitirq
pitirq:
	movb $0xC2, %al
	movw $0x43, %dx
	outb %al, %dx
	movw $0x40, %dx
	inb %dx, %al
	andb $0x3E, %al
	cmpb $0x30, %al
	je int_on_term_count
	jmp square_gen

int_on_term_count:
	lcall $0x8, $oneshot_handler
	popl %edx
	iret

square_gen:
	lcall  $0x8, $square_handler
	popl %edx
	iret

.global apic_timer_irq
apic_timer_irq:
	lcall $0x8, $apic_timer_handler
	popl %edx
	iret
