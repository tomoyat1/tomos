//Entry points for irq handlers.

.global divided_by_zero
divided_by_zero:
	lcall $0x8, $divzero
	iret
