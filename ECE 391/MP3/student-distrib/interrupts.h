/*
*	interrupts.h - Function Header File to be used with "interrupts.S"
*/

#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

/* Keyboard interrupt asm wrapper */
extern void keyboard_handler_0();

/* Rtc interrupt wrapper */
extern void rtc_handler_0();

/* Syscall interrupt wrapper */
extern void syscall_0();


#endif /* INTERRUPT_HANDLER_H */
