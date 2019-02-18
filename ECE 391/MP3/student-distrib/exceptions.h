#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include "x86_desc.h"

#define EXCEPTION_RET_CODE 0x100

#define EXCEPTION_DIV_BY_ZERO 0x00
#define EXCEPTION_SSI 0x01
#define EXCEPTION_NMI 0x02
#define EXCEPTION_BREAKPOINT 0x03
#define EXCEPTION_OVERFLOW 0x04
#define EXCEPTION_BOUND_RANGE_EXCEEDED 0x05
#define EXCEPTION_INVALID_OPCODE 0x06
#define EXCEPTION_DEVICE_NOT_AVAILABLE 0x07
#define EXCEPTION_DOUBLE_FAULT 0x08
#define EXCEPTION_COPROCESSOR_SEG_OVERRUN 0x09
#define EXCEPTION_INVALID_TSS 0x0A
#define EXCEPTION_SEGMENT_NOT_PRESENT 0x0B
#define EXCEPTION_STACK_SEGMENT_FAULT 0x0C
#define EXCEPTION_GENERAL_PROTECTION_FAULT 0x0D
#define EXCEPTION_PAGE_FAULT 0x0E
#define EXCEPTION_FLOATING_POINT 0x10
#define EXCEPTION_ALIGNMENT_CHECK 0x11
#define EXCEPTION_MACHINE_CHECK 0x12
#define EXCEPTION_VIRTUALIZATION 0x14
#define EXCEPTION_SECURITY 0x1E

#define SYSCALL_VECTOR		0x80
#define RTC_VECTOR			0x28
#define KEYBOARD_VECTOR		0x21

void initialize_exceptions();

void divide_by_zero_handler();
void single_step_interrupt();
void non_maskable_interrupt();
void breakpoint();
void overflow();
void bound_range_exceeded_handler();
void invalid_opcode_handler();
void device_not_available_handler();
void double_fault_handler();
void coprocessor_segment_overrun();
void invalid_tss_handler();
void segment_not_present_handler();
void stack_segment_fault_handler();
void general_protection_fault_handler();
void page_fault_handler();
void floating_point_handler();
void alignment_check_handler();
void machine_check_handler();
void virtualization_handler();
void security_handler();

#endif
