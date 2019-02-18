#include "lib.h"
#include "pcb.h"
#include "page.h"
#include "system_calls.h"
#include "terminals.h"

unsigned int num_tasks_each_terminal[NUM_TERMINALS] = {0};

/*
 * terminals_init
 * DESCRIPTION: Initialize multiple terminals
 * INPUT: none
 * OUTPUT: none
 * RETURN VALUE: none
 * SIDE EFFECT: none
 */
void terminals_init()
{
	int i;
	for (i = 0; i < NUM_TERMINALS; i++)
		terminals[i].active_task = NULL;
		
	cur_terminal = 0;
}

/* change_terminal
 * DESCRIPTION: Switch to target terminal
 * INPUT: terminal - index of target terminal
 * OUTPUT: none
 * RETURN VALUE: none
 * SIDE EFFECT: switches the active terminal
 */

void change_terminal(int32_t terminal)
{
	/* Boundary check and check if switch is necessary */
	if (terminal >= NUM_TERMINALS || cur_terminal == terminal)
		return;
		
	/* get PID of target terminal */
	uint32_t pid;
	asm volatile ("bsf %1,%0"
	: "=r" (pid)
	: "r" (pid_map)
	: "cc");
	/* Check if PID is available */
	if (pid >= MAX_PROCESSES)
	{
		printf("Can not allocate PID\n");
		return;
	}
	
	/* Store current video memory */
	memcpy(&(terminals[cur_terminal].video_status), (void*)VIDEO, VIDEO_MEM_SIZE);
	
	/* Store current cursor position */
	save_cursor(&(terminals[cur_terminal].cursor_x), &(terminals[cur_terminal].cursor_y));
	
	/* Store current program control block */
	terminals[cur_terminal].active_task = cur_pcb;
	
	/* Store ESP and EBP */
	asm volatile ("movl %%esp, %0 \n\
	movl %%ebp, %1"
	: "=r" (terminals[cur_terminal].cur_esp), "=r" (terminals[cur_terminal].cur_ebp)
	:
	: "cc");
	
	/* Switch to target terminal */
	terminals[terminal].prev_terminal = cur_terminal;
	cur_terminal = terminal;
	terminal_t* term = &terminals[terminal];


	/* see if terminal is initially ran, and execute new shell if it is */
	if (term->active_task == NULL)
	{
		/* Clear screen */
		clear();
		update_cursor(0,0);
		printf("Terminal %u\n\n", cur_terminal);
		/* execute new shell */
		cur_pcb = NULL;
		execute((uint8_t*)"shell");
		return;
	}

	/* Get video memory from stored value */
	memcpy((void*)VIDEO, &(terminals[cur_terminal].video_status), VIDEO_MEM_SIZE);
	
	/* Restore cursor position */
	store_cursor(terminals[cur_terminal].cursor_x, terminals[cur_terminal].cursor_y);
	update_cursor(terminals[cur_terminal].cursor_x, terminals[cur_terminal].cursor_y);

	/* Get PCB of the program on target terminal */
	cur_pcb = terminals[cur_terminal].active_task;
	
	/* Get corresponding PID to user memory */
	map_user_program(cur_pcb->pid);
	
	/* return to user mode */
	asm volatile ("movl %0, %%esp \n\
	movl %1, %%ebp"
	:
	: "r" (terminals[cur_terminal].cur_esp), "r" (terminals[cur_terminal].cur_ebp)
	: "cc");
	
	return;
}
