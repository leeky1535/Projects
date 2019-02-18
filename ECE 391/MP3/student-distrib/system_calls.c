/*
*   system_calls.c - used to handle all system calls at int = 0x80
*/

#include "x86_desc.h"
#include "lib.h"
#include "system_calls.h"
#include "rtc.h"
#include "op_table.h"
#include "pcb.h"
#include "filesys.h"
#include "page.h"
#include "terminals.h"

/* 
*	halt
*   DESCRIPTION: halt program
*   INPUT: status - status code from program
*   OUTPUT: none
*   RETURN VALUE: never gets to the return statement. return -1 in case of errors, that make it reach return statement.
*   SIDE EFFECT: none
*/
int32_t halt(uint8_t status){   
	halt_program((uint16_t)status);
	return -1;
}


/* 
*	execute
*   DESCRIPTION: execute system call.
*   INPUT: command - determines which process to execute.
*   OUTPUT: none
*   RETURN VALUE: return value from load_program
*   SIDE EFFECT: none
*/
int32_t execute(const uint8_t* command){
    uint32_t len = strlen((char*)command);

	static uint8_t fname [LINE_BUFFER_LENGTH];
	uint8_t* args;
	
	memcpy(fname, command, len + 1);       //copy const command to fname
	args = fname + len;

	int i;

	for(i = 0; i < len; i++)
	{
		if(command[i] == ' ')           //found the end of command
		{
			fname[i] = '\0';
			args = fname + i + 1; 
			break;
		}
	}
	return load_program(fname, args);            //load the program
}


/*
 *  load_program
 *  DESCRIPTON: loads the corresponding program, helper for execute
 *  INPUT: fname - filename
 *         argument - argument from buffer
 *  OUTPUT: loads the program
 *  RETURN VALUE: -1 - error, retval - success
 *  SIDE EFFECT: none
 */
int32_t load_program(const uint8_t* fname, uint8_t* argument)
{
	/* get corresponding directory entry */
	dentry_t f_dentry;
	if (read_dentry_by_name((char*)fname, &f_dentry) == -1)
	{
		printf("No Such File!");
		return -1;
	}
	
	/* check if file is executable type */
	if (f_dentry.file_type != FILE_TYPE_FILE)
	{
		printf("Not executable");
		return -1;
	}
	
	uint8_t header[HEADER_SIZE];
	
	/* check for ELF header in file */
	read_data(f_dentry.inode_num, 0, header, HEADER_SIZE);
	if (header[0] != ELF_MAGIC_NUM_0 || header[1] != ELF_MAGIC_NUM_1 || header[2] != ELF_MAGIC_NUM_2 || header[3] != ELF_MAGIC_NUM_3)
	{
		printf("Not executable");
		return -1;
	}
	
	/* Get the entry location, located from 24th byte of the header */
	void* entry_loc = (void*)(*(uint32_t*)(header + 24));
	
	/* Get Program ID from bitmap */
	uint32_t pid;
	asm volatile ("bsf %1,%0"
	: "=r" (pid)
	: "r" (pid_map)
	: "cc");
	if (pid >= MAX_PROCESSES)
	{
		printf("Can not allocate PID");
		return -1;
	}
	
	/* Set PID */
	pid_map ^= (1<<pid);
	num_tasks_each_terminal[cur_terminal]++;

	/* map pid */
	map_user_program(pid);
	
	/* read the data from program */
	read_data(f_dentry.inode_num, 0, (void*)PROGRAM_IMG_ADDR, PDE_SIZE_LARGE);
	/* get new address for pcb */
	pcb_t* new_pcb = (pcb_t*)(2 * PDE_SIZE_LARGE - (pid + 1) * KERNEL_STACK_LEN);

	/* initialize new pcb */
	new_pcb->pid = pid;

	/* Initialize Standard Input/Output */
	new_pcb->fd_table[0].fops_table = &std_in_ops;
	new_pcb->fd_table[0].inode = 0;
	new_pcb->fd_table[0].pos = 0;
	new_pcb->fd_table[0].flags = 1;
	
	new_pcb->fd_table[1].fops_table = &std_out_ops;
	new_pcb->fd_table[1].inode = 0;
	new_pcb->fd_table[1].pos = 0;
	new_pcb->fd_table[1].flags = 1;
	
	new_pcb->fdbmp = 0xFFC;
	int i;
	for (i = 2; i < FILE_DESC_NUM; i++) //FDs 2-7 are free
	{
		new_pcb->fd_table[i].fops_table = 0;
		new_pcb->fd_table[i].inode = 0;
		new_pcb->fd_table[i].pos = 0;
		new_pcb->fd_table[i].flags = 0;
	}

	/* Root */
	if (pid == 0)
	{
		new_pcb->parent_pcb = NULL;
		new_pcb->parent_pcb = NULL;
	}
	else
	{
		new_pcb->parent_pcb = cur_pcb;
		new_pcb->parent_esp0 = tss.esp0;
	}

	/* Manipulate TSS */
	tss.esp0 = 2 * PDE_SIZE_LARGE - pid * KERNEL_STACK_LEN - 4;
	/* Update PCB */
	cur_pcb = new_pcb;

	/* save arguments to pcb */
	strcpy(cur_pcb->arg_buf, (char*)argument);

	/* save esp and ebp to pcb */
	asm volatile ("movl %%esp, %0 \n\
	movl %%ebp, %1"
	: "=r" (cur_pcb->old_esp), "=r" (cur_pcb->old_ebp)
	:
	: "cc");

    uint32_t retval;

    /* IRET */
	asm volatile ("movl %1, %%eax \n\
	movw %%ax, %%ds \n\
	pushl %1 \n\
	pushl %2 \n\
	pushfl \n\
	popl %%eax \n\
	orl $0x00000200 , %%eax \n\
	pushl %%eax \n\
	pushl %3 \n\
	pushl %4 \n\
	iret \n\
	ret_halt: movl %%eax, %0 \n\
	"
	: "=r" (retval)
	: "r" (USER_DS), "r" (USER_STACK_BOTTOM), "r" (USER_CS), "r" (entry_loc)
	: "cc", "eax");
	return retval;
}


/*
 * halt_program
 * DESCRIPTION: Halt and return to parent
 * INPUT: status - status code from program or exception
 * OUTPUT: none
 * RETURN VALUE: none
 * SIDE EFFECT: none
 */
void halt_program(uint16_t status)
{
	uint32_t pid = cur_pcb->pid;
	/* exitting root shell */
	if (pid == 0)
	{
		pid_map |= (1 << pid);
		printf("No Program Running\n");
		execute((uint8_t*)"shell");
		return;
	}
	if(num_tasks_each_terminal[cur_terminal] == 1){
		terminals[cur_terminal].active_task = NULL;
		change_terminal(prev_terminal);
	}
	uint32_t old_esp = cur_pcb->old_esp;
	uint32_t old_ebp = cur_pcb->old_ebp;

	/* free pid */
	pid_map |= (1 << pid);
	num_tasks_each_terminal[cur_terminal]--;

	/* reset TSS */
	tss.esp0 = cur_pcb->parent_esp0;
	cur_pcb = cur_pcb->parent_pcb;
	/* map user memory back to original pcb */
	map_user_program(cur_pcb->pid);
	
	
	asm volatile("movl %1, %%esp \n\
	movl %2, %%ebp \n\
	xorl %%eax, %%eax \n\
	mov %0, %%ax \n\
	jmp ret_halt"
	:
	: "r" (status), "r" (old_esp), "r" (old_ebp)	
	: "cc");

}


/* 
*	read
*   DESCRIPTION: read data from devices
*   INPUT: fd - file descriptor
*          buf - buffer
*          nbytes - size of the buffer
*   OUTPUT: none
*   RETURN VALUE: number of bytes read.
*   SIDE EFFECT: none
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes){
	/* Boundary Check */
	if (fd < 0 || fd > FILE_DESC_NUM)
		return -1;
	/* NULL Check */
	if (buf == NULL)
		return -1;
	/* check if the file is being used */
	if (cur_pcb->fd_table[fd].flags == 0)
		return -1;

	return cur_pcb->fd_table[fd].fops_table->read(fd, (char*)buf, nbytes);
}


/* 
*	write
*   DESCRIPTION: write to device
*   INPUT: fd - file descriptor
*          buf - buffer
*          nbytes - size of the buffer
*   OUTPUT: none
*   RETURN VALUE: number of bytes written.
*   SIDE EFFECT: none
*/
int32_t write(int32_t fd, const void* buf, int32_t nbytes){
	/* Boundary Check */
	if (fd < 0 || fd >= FILE_DESC_NUM)
		return -1;
	/* NULL Check */
	if (buf == NULL)
		return -1;
	/* check if the file is being used */
	if (cur_pcb->fd_table[fd].flags == 0)
		return -1;

	return cur_pcb->fd_table[fd].fops_table->write(fd, (char *)buf, nbytes);
}

/* 
*	open
*   DESCRIPTION: opens a file from filesystem
*   INPUT: filename - file to be opened
*   OUTPUT: opens the file
*   RETURN VALUE: fd - file descriptor
*   SIDE EFFECT: none
*/
int32_t open(const uint8_t* filename){
	/* Does file exist? */
	dentry_t file;
	/* NOOOOOO */
	if (read_dentry_by_name((char*)filename, &file) != 0)
		return -1;
	
	/* get new file descriptor */
	int32_t fd = get_fd();
	/* no new file descriptor */
	if (fd == -1)
		return -1;
	
	/* get the corresponding device */
	uint32_t ftype = file.file_type;
	cur_pcb->fd_table[fd].fops_table = op_table_table[ftype];
	cur_pcb->fd_table[fd].inode = file.inode_num;
	cur_pcb->fd_table[fd].pos = 0;
	cur_pcb->fd_table[fd].flags = 1;

	cur_pcb->fd_table[fd].fops_table->open(filename);
	return fd;
}


/* 
*	close
*   DESCRIPTION: close file
*   INPUT: fd - file descriptor to be closed
*   OUTPUT: close file
*   RETURN: 0 - success, -1 - error
*   SIDE EFFECT: none
*/
int32_t close (int32_t fd){
	/* Boudnary Check */
	if (fd < 2 || fd > FILE_DESC_NUM)
		return -1;
		
	/* Is file being used? */
	if (cur_pcb->fd_table[fd].flags == 0)
		return -1;

	cur_pcb->fd_table[fd].fops_table->close(fd);
	cur_pcb->fd_table[fd].flags = 0;

	free_fd(fd);
	
	return 0;
}

/* 
*	Function getargs()
*   Description: get arguments to the buffer
*   input: buf - destination buffer
*          nbytes - number of bytes of the buffer
*   output: arguments into buffer
*/
int32_t getargs (uint8_t* buf, int32_t nbytes) {
	/* Boundary , NULL Check */
	if(nbytes <= 0)
		return -1;
	if(cur_pcb->arg_buf == NULL)
		return -1;
	if(strlen(cur_pcb->arg_buf) > nbytes)
	{
		return -1;
	}
	if (strlen(cur_pcb->arg_buf) == 0)
		return -1; 
	strncpy((int8_t*)buf, (int8_t*)cur_pcb->arg_buf, nbytes);

	return 0;
}

/* 
*	Function vidmap()
*	Description: maps the text-mode video memory into user space at a pre-set virtual address
*	input: 	screen_start -- pointer to start of user video memory
*	output: returns virtual address on success, -1 on failure
*	effect: effects the physical video memory buffer 
*/
int32_t vidmap(uint8_t ** screen_start){
	/* boundary check */
	if ((uint32_t)screen_start < USER_VIRT_ADDR || (uint32_t)screen_start > USER_VIRT_ADDR + PDE_SIZE_LARGE)
		return -1;
	
	/* map the screen start pointer */
	*screen_start = (uint8_t*)USER_VID_ADDR;

	return USER_VID_ADDR;
}


/* 
*	Function set_handler() - not implemented yet
*/
int32_t set_handler(int32_t signum, void* handler_address){
	return 0;
}

/* 
*	Function sigreturn() - not implemented yet
*/
int32_t sigreturn(void){
	return 0;
}
