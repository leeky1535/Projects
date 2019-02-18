/* keyboard.c - the H part of the keyboard
 * vim:ts=4 no expand tab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */
#include "types.h"
#include "i8259.h"
#include "lib.h"
#include "keyboard.h"
#include "tests.h"
#include "rtc.h"
#include "filesys.h"
#include "system_calls.h"
#include "terminals.h"


unsigned int shift_flag = 0;
unsigned int caps_flag = 0;
unsigned int ctrl_flag = 0;
unsigned int alt_flag = 0;
volatile int enter_flag = 0;
static int buffer_idx[NUM_TERMINALS] = {0,0,0};
unsigned int glob_idx_val = 0;
static char buffer[NUM_TERMINALS][BUFFER_SIZE];
//uint8_t buffer_for_test[BUFFER_SIZE];
//int32_t buffer_break_number_for_test = 1000;

/* char list for given condition and scancode*/
static uint8_t char_list[4][KEYBOARD_INPUT] = {
	//default
   {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l' , ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm',',', '.', '/', '\0', '*', '\0', ' ', '\0'},
   //shift pressed
   {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\0', '\0', 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L' , ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', '\0', '*', '\0', ' ', '\0'},
   //caps lock pressed
   {'\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\0', '\0', 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L' , ';', '\'', '`', '\0', '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', '\0', '*', '\0', ' ', '\0'},
   // caps lock , shift pressed
   {'\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\0', '\0', 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l' , ':', '"', '~', '\0', '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', '<', '>', '?', '\0', '*', '\0', ' ', '\0'}
};

/*
keyboard_init
- initialize keyboard
@param none
@return none
*/
void keyboard_init(){
	enable_irq(KBD_IRQ_NUM);
}


/*
 * int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes)
 *   DESCRIPTION: read system call for terminal driver. Reads until the newline character is seen, or nbytes from the buffer.
 *   INPUTS: fd -- file descriptor index
 *           buf -- buffer to fill
 *           nbytes -- the number of bytes to read
 *   RETURN VALUE: bytes read, -1 upon error.
 */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
	while (enter_flag == 0) {}
	enter_flag = 0;

	update_cursor_enter();

	int i;

	for(i=0; i < nbytes && i < BUFFER_SIZE; i++)
	{
		((char*)buf)[i] = buffer[cur_terminal][i];
		if (buffer[cur_terminal][i] == NULL)
		{
			((char*)buf)[i] = '\0';
			break;
		}
	}

	clear_buffer();

	return i+1;
}

void clear_buffer(){
	int i;
	for(i = 0; i <BUFFER_SIZE; i++){
		buffer[cur_terminal][i] = '\0';
	}
	buffer_idx[cur_terminal] = 0;
}


/*
 * int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes)
 *   DESCRIPTION: all datas in buffer is shown on display
 *   INPUTS: fd -- file descriptor
 *           buf -- buffer to write with
 *           nbytes -- the number of bytes to display
 *   RETURN VALUE: bytes written, -1 upon error.
 *   SIDE EFFECTS: none
 */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes) {
	int i;
	for(i = 0; i < nbytes; i++){
		if (((char*)buf)[i] == '\0')
			break;
		putc(((char*)buf)[i]);
	}
	return i;
}

int32_t terminal_open(const uint8_t * filename){
	return 0;
}

int32_t terminal_close(int32_t fd){
	return 0;
}
/*
keyboard_handler
- takes input value from the port and prints on screen
@param none
@return none
*/
void keyboard_handler(){
	//critical section starts
	cli();

	unsigned char scancode = 0;
	do {
		if (inb(KBD_PORT) != 0) {
			scancode = inb(KBD_PORT);
			if (scancode > 0) {
				break;
			}
		}
	} while(1);
	

  //check for special inputs
  switch (scancode) {
  	case ENTER:
  	  enter_flag = 1;
  	  break;
    case CAPS_PRESSED:
      caps_flag = !(caps_flag);
      break;
    case LSHIFT_PRESSED:
      shift_flag = 1;
      break;
    case LSHIFT_RELEASED:
      shift_flag = 0;
      break;
    case RSHIFT_PRESSED:
      shift_flag = 1;
      break;
    case RSHIFT_RELEASED:
      shift_flag = 0;
      break;
    case CTRL_PRESSED:
      ctrl_flag = 1;
      break;
    case CTRL_RELEASED:
      ctrl_flag = 0;
	  break;
	case ALT_PRESSED:
      alt_flag = 1;
      break;
    case ALT_RELEASED:
      alt_flag = 0;
	  break;
    default:
      send_to_buffer(scancode);
      break;
  }

	send_eoi(KBD_IRQ_NUM);
	
	//critical section ends
	sti();
}



/*
 * send_to_buffer(uint8_t scancode)
 *   DESCRIPTION: Checks for enter, backspace and ctrl+l operations. Else prints the entered key from the keyboard on display. Characters are pushed into the buffer.
 *   INPUTS: uint8_t scancode -- input from keyboard
 *   RETURN VALUE: NONE
 */
void send_to_buffer(uint8_t scancode){

	uint8_t key;

	if(buffer_idx[cur_terminal] == (BUFFER_SIZE-1)){
		buffer[cur_terminal][buffer_idx[cur_terminal]] = '\0';
		buffer_idx[cur_terminal]++;
		update_cursor_enter();
		//buffer overflow check case
		if(buffer_idx[cur_terminal] == BUFFER_SIZE){
			buffer_idx[cur_terminal] = 0;
			return;
		}
	}


	//Checking for unknown/invalid scancode
	if(scancode < KEYBOARD_INPUT){
		if(shift_flag == 1 && caps_flag == 0){
			key = char_list[1][scancode];
		}
		else if(shift_flag == 0 && caps_flag == 1){
			key = char_list[2][scancode];
		}
		else if(shift_flag == 1 && caps_flag == 1){
			key = char_list[3][scancode];
		}
		else{
			key = char_list[0][scancode];
		}
	}
	else{
		key = '\0';
		return;
	}
	if(scancode != BACKSPACE && key == '\0')
		return;

	//ctrl + L pressed, clear screen and buffer
	if((ctrl_flag == 1 ) && (key == 'l' )){
		//set cursur to (0,0) and clear buffer and clear the screen and reinitialize buffer index
		clear();
		update_cursor(0, 0);
	}
	else if((ctrl_flag == 1 ) && (key == 'w' )){
		//write buffer to screen
		update_cursor_enter();
		terminal_write(1, buffer, BUFFER_SIZE);
	}
	else if((ctrl_flag == 1 ) && (key == 't' )){
		update_cursor_enter();
		clear_buffer();
		execute((uint8_t*)"testprint");
	}
	else if((alt_flag == 1) && (key == '1')){
		send_eoi(KBD_IRQ_NUM);
		sti();
		change_terminal(0);
		return;
	}
	else if((alt_flag == 1) && (key == '2')){
		send_eoi(KBD_IRQ_NUM);
		sti();
		change_terminal(1);
		return;
	}
	else if((alt_flag == 1) && (key == '3')){
		send_eoi(KBD_IRQ_NUM);
		sti();
		change_terminal(2);
		return;
	}

	//BACKSPACE pressed, delete one char, move cursor back
	else if((scancode == BACKSPACE) && (buffer_idx[cur_terminal] >= 0) && (buffer_idx[cur_terminal] < (BUFFER_SIZE-1))){
		//filter out conditoin case where index 0 causes space error with backspace.
		if(buffer[cur_terminal][buffer_idx[cur_terminal]-1] != '\0' && buffer_idx[cur_terminal] != 0){
			buffer_idx[cur_terminal] = buffer_idx[cur_terminal]-1;		
			buffer[cur_terminal][buffer_idx[cur_terminal]] = '\0';
			update_cursor_bs();
		}
	}
	//add char to buffer
	else if(buffer_idx[cur_terminal] < (BUFFER_SIZE-1)){
		buffer[cur_terminal][buffer_idx[cur_terminal]] = key;
		putc(key);
		buffer_idx[cur_terminal]++;		
	}

	return;
}


