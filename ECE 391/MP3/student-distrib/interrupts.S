#	interrupts.S - Interrupt handler for rtc/keyboard

#define ASM 1
#include "x86_desc.h"

.GLOBL keyboard_handler_0
keyboard_handler_0:
	pushal
	pushfl
	call keyboard_handler
	popfl
	popal
	iret

.GLOBL rtc_handler_0
rtc_handler_0:
	pushal
	pushfl
	call rtc_handler
	popfl
	popal
	iret

