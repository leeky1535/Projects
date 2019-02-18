#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "lib.h"
#include "pcb.h"
#include "system_calls.h"

#define NUM_TERMINALS 3
//From lib.h
#define VIDEO_MEM_SIZE (NUM_COLS * NUM_ROWS * 2)

/* Terminal control block */
typedef struct terminal_struct {
	uint32_t cur_esp;
	uint32_t cur_ebp;
	pcb_t* active_task; //NULL for no task
	uint8_t video_status [VIDEO_MEM_SIZE];
	int cursor_x;
	int cursor_y;
	uint8_t prev_terminal;
} terminal_t;

/* Array to store terminal status */
terminal_t terminals [NUM_TERMINALS];

unsigned int num_tasks_each_terminal[NUM_TERMINALS];
uint8_t cur_terminal;
uint8_t prev_terminal;

void terminals_init();
void change_terminal(int32_t terminal);

#endif
