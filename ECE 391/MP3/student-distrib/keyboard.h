 /* keyboard.h - the H part of the keyboard
 * vim:ts=4 no expand tab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */
 #ifndef _KEYBOARD_H
 #define _KEYBOARD_H

#include "types.h"
#include "lib.h"
#include "i8259.h"

#define BUFFER_SIZE 128
#define KEYBOARD_INPUT 0x3D
#define LSHIFT_PRESSED 0x2A
#define RSHIFT_PRESSED 0x36
#define LSHIFT_RELEASED 0xAA
#define RSHIFT_RELEASED 0xB6
#define CTRL_PRESSED	0x1D
#define CTRL_RELEASED   0x9D
#define ALT_PRESSED   0x38
#define ALT_RELEASED    0xB8

#define CAPS_PRESSED 0x3A
#define BACKSPACE 0x0E
#define ENTER 0x1C
/*
#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
*/
 
 
#define KBD_PORT 0x60
#define KBD_IRQ_NUM 1


/* Initializes the keyboard */
extern void keyboard_init(void);
 
/* Handler for keyboard interrupt */
extern void keyboard_handler(void);

/* Helper function to create buffer from keyboard input */
void send_to_buffer(uint8_t scancode);
void clear_buffer();
/* Functions for terminal */
//TODO: In the future might be better to create seperate files
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
int32_t terminal_open(const uint8_t * filename);
int32_t terminal_close(int32_t fd);

#endif /* _KEYBOARD_H */

