/*
*	system_calls.h - Function Header File to be used with system_calls.c
*/
#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H

#define LINE_BUFFER_LENGTH 128
#define KERNEL_STACK_LEN 0x1000

#define EXCEPTION_STATUS 0x100

#define ELF_MAGIC_NUM_0 0x7F                  //Magic numbers for ELF
#define ELF_MAGIC_NUM_1 0x45
#define ELF_MAGIC_NUM_2 0x4C
#define ELF_MAGIC_NUM_3 0x46
#define HEADER_SIZE     40

int32_t load_program(const uint8_t* fname, uint8_t* argument);
void halt_program(uint16_t status);

/* Halt System Call */
int32_t halt (uint8_t status);

/* Execute System Call */
int32_t execute (const uint8_t* command);

/* Read System Call */
int32_t read (int32_t fd, void* buf, int32_t  nbytes);

/* Write System Call */
int32_t write (int32_t fd, const void* buf, int32_t nbytes);

/* Open System Call */
int32_t open (const uint8_t* filename);

/* Close System Call */
int32_t close (int32_t fd);

/* Getargs System Call */
int32_t getargs (uint8_t* buf, int32_t nbytes);

/* Vidmap System Call */
int32_t vidmap (uint8_t** screen_start);

/* Set Handler System Call */
int32_t set_handler (int32_t signum, void* handler_address);

/* Sigreturn System Call */
int32_t sigreturn (void);

/* Syscall interrupt wrapper */
void syscall_0();

#endif
