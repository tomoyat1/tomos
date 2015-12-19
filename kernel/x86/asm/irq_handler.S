//Entry points for irq handlers.

.global divided_by_zero
divided_by_zero:
	pushal
	cld
	call divzero
	popal
	iret

.global kbdirq
kbdirq:
	pushal
	cld
	call keypress
	popal
	iret

.global pitirq
pitirq:
	pushal
	cld
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
	call oneshot_handler
	jmp pit_irq_exit

square_gen:
	call  square_handler
	jmp pit_irq_exit

pit_irq_exit:
	popal
	iret

.global apic_timer_irq
apic_timer_irq:
	pushal
	cld
	call apic_timer_handler
	popal
	iret

.global page_fault
page_fault:
	pushl $0xDEADBEEF
	cli
	hlt
	iret
