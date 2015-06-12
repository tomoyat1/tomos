//Entry points for irq handlers.

.global divided_by_zero
divided_by_zero:
	lcall $0x8, $divzero
	popl %edx
	iret
.global kbdirq
kbdirq:
	lcall $0x8, $keypress
	popl %edx
	iret
.global pitirq
pitirq:
	lcall  $0x8, $pit_handler
	popl %edx
	iret

