#ifndef _PCB_H
#define _PCB_H

#include "filesys.h"
/* Bitmap of PIDs currently begin used */
uint32_t pid_map;

/* We are limited to running six tasks at once */
#define MAX_PROCESSES 6
#define MAX_BUF_SIZE 100

typedef struct pcb_t
{	
	uint32_t pid; //Process ID
	uint32_t tid; //Terminal ID for multiple terminal
	
	uint32_t fdbmp; //Bitmap of used file descriptors
	file_desc_t fd_table [FILE_DESC_NUM];
	struct pcb_t* parent_pcb;
	uint32_t parent_esp0;
	uint32_t old_esp;
	uint32_t old_ebp;
	char arg_buf[ARG_BUFFER_SIZE]; 
} pcb_t;

pcb_t* cur_pcb;

#endif
