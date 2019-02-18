#include "exceptions.h"
#include "lib.h"
#include "i8259.h"
#include "interrupts.h"
#include "x86_desc.h"
#include "keyboard.h"
#include "rtc.h"
#include "system_calls.h"
#include "interrupts.h"

//Initialize all x86 exceptions in the IDT
void initialize_exceptions()
{
	SET_IDT_ENTRY(idt[EXCEPTION_DIV_BY_ZERO], divide_by_zero_handler);
    SET_IDT_ENTRY(idt[EXCEPTION_SSI], single_step_interrupt);
    SET_IDT_ENTRY(idt[EXCEPTION_NMI], non_maskable_interrupt);
    SET_IDT_ENTRY(idt[EXCEPTION_BREAKPOINT], breakpoint);
    SET_IDT_ENTRY(idt[EXCEPTION_OVERFLOW], overflow);
	SET_IDT_ENTRY(idt[EXCEPTION_BOUND_RANGE_EXCEEDED], bound_range_exceeded_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_INVALID_OPCODE], invalid_opcode_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_DEVICE_NOT_AVAILABLE], device_not_available_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_DOUBLE_FAULT], double_fault_handler);
    SET_IDT_ENTRY(idt[EXCEPTION_COPROCESSOR_SEG_OVERRUN], coprocessor_segment_overrun);
	SET_IDT_ENTRY(idt[EXCEPTION_INVALID_TSS], invalid_tss_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_SEGMENT_NOT_PRESENT], segment_not_present_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_STACK_SEGMENT_FAULT], stack_segment_fault_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_GENERAL_PROTECTION_FAULT], general_protection_fault_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_PAGE_FAULT], page_fault_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_FLOATING_POINT], floating_point_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_ALIGNMENT_CHECK], alignment_check_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_MACHINE_CHECK], machine_check_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_VIRTUALIZATION], virtualization_handler);
	SET_IDT_ENTRY(idt[EXCEPTION_SECURITY], security_handler);

    SET_IDT_ENTRY(idt[RTC_VECTOR], rtc_handler_0);      //RTC Interrupt Handler
    SET_IDT_ENTRY(idt[KEYBOARD_VECTOR], keyboard_handler_0);     //Keyboard Interrupt Handler
    SET_IDT_ENTRY(idt[SYSCALL_VECTOR], syscall_0);         //Syscall handler
    idt[SYSCALL_VECTOR].dpl = 3; //We also need to make syscalls runnable in ring 3
    idt[SYSCALL_VECTOR].reserved3 = 1; //Set type to 0xF (Trap gate)
}

void divide_by_zero_handler()
{
    printf ("EXCEPTION: Attempted division by zero!\n");
    halt_program(EXCEPTION_STATUS);
}

void single_step_interrupt()
{
    printf ("EXCEPTION: Single Step Interrupt!\n");
    halt_program(EXCEPTION_STATUS);
}

void non_maskable_interrupt()
{
    printf ("EXCEPTION: Non Maskable Interrupt!\n");
    halt_program(EXCEPTION_STATUS);
}

void breakpoint()
{
    printf ("EXCEPTION: Breakpoint!\n");
    halt_program(EXCEPTION_STATUS);
}

void overflow()
{
    printf ("EXCEPTION: Overflow!\n");
    halt_program(EXCEPTION_STATUS);
}

void bound_range_exceeded_handler()
{
    printf ("EXCEPTION: Bound range exceeded!\n");
    halt_program(EXCEPTION_STATUS);
}

void invalid_opcode_handler()
{
    printf ("EXCEPTION: invalid opcode!\n");
    halt_program(EXCEPTION_STATUS);
}

void device_not_available_handler()
{
    printf ("EXCEPTION: Device not available!\n");
    halt_program(EXCEPTION_STATUS);
}

void double_fault_handler()
{
    printf ("EXCEPTION: Double fault!\n");
    halt_program(EXCEPTION_STATUS);
}

void coprocessor_segment_overrun()
{
    printf ("EXCEPTION: Coprocessor Segment Overrun!\n");
    halt_program(EXCEPTION_STATUS);
}

void invalid_tss_handler()
{
    printf ("EXCEPTION: Indvalid TSS!\n");
    halt_program(EXCEPTION_STATUS);
}

void segment_not_present_handler()
{
    printf ("EXCEPTION: Segment not present!\n");
    halt_program(EXCEPTION_STATUS);
}

void stack_segment_fault_handler()
{
    printf ("EXCEPTION: Stack segment fault!\n");
    halt_program(EXCEPTION_STATUS);
}

void general_protection_fault_handler()
{
    printf ("EXCEPTION: General protection fault!\n");
    halt_program(EXCEPTION_STATUS);
}

void page_fault_handler()
{
    printf ("EXCEPTION: Page fault!\n");
    halt_program(EXCEPTION_STATUS);
}

void floating_point_handler()
{
    printf ("EXCEPTION: Floating point exception!\n");
    halt_program(EXCEPTION_STATUS);
}

void alignment_check_handler()
{
    printf ("EXCEPTION: Alignment check!\n");
    halt_program(EXCEPTION_STATUS);
}

void machine_check_handler()
{
    printf ("EXCEPTION: Machine check!\n");
    halt_program(EXCEPTION_STATUS);
}

void virtualization_handler()
{
    printf ("EXCEPTION: Virtualization exception!\n");
    halt_program(EXCEPTION_STATUS);
}

void security_handler()
{
    printf ("EXCEPTION: Security exception!\n");
    halt_program(EXCEPTION_STATUS);
}
