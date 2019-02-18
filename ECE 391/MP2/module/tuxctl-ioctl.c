/* tuxctl-ioctl.c
 *
 * Driver (skeleton) for the mp2 tuxcontrollers for ECE391 at UIUC.
 *
 * Mark Murphy 2006
 * Andrew Ofisher 2007
 * Steve Lumetta 12-13 Sep 2009
 * Puskar Naha 2013
 */

#include <asm/current.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/miscdevice.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
#include <linux/spinlock.h>

#include "tuxctl-ld.h"
#include "tuxctl-ioctl.h"
#include "mtcp.h"


#define debug(str, ...) \
	printk(KERN_DEBUG "%s: " str, __FUNCTION__, ## __VA_ARGS__)

struct tux_lock{
	spinlock_t buttons_lock;
	unsigned long buttons;
};

const static unsigned char seven_segment [16] = {0xE7, 0x06, 0xCB, 0x8F, 0x2E, 0xAD, 0xED, 0x86, 0xEF, 0xAF, 0xEE, 0x6D, 0xE1, 0x4F, 0xE9, 0xE8};

static struct tux_lock button_status;
static unsigned long led_status;
unsigned a, b, c;
static unsigned int ack_flag;
static unsigned int busy = 0;

int tuxctl_ioctl_init(struct tty_struct* tty);
int tuxctl_ioctl_buttons(struct tty_struct* tty, unsigned long arg);
int tuxctl_ioctl_set_led (struct tty_struct* tty, unsigned long arg);
int tuxctl_ioctl_led_request(struct tty_struct* tty);
int tuxctl_ioctl_read_led (struct tty_struct* tty, unsigned long arg);
int tuxctl_ioctl_get_button(unsigned b, unsigned c);
/************************ Protocol Implementation *************************/

/************************ Protocol Implementation *************************/

/* tuxctl_handle_packet()
 * IMPORTANT : Read the header for tuxctl_ldisc_data_callback() in 
 * tuxctl-ld.c. It calls this function, so all warnings there apply 
 * here as well.
 */
void tuxctl_handle_packet (struct tty_struct* tty, unsigned char* packet)
{
	if(busy) return;

    a = packet[0]; /* Avoid printk() sign extending the 8-bit */
    b = packet[1]; /* values when printing them. */
    c = packet[2];

    switch(a)
    {
     	case MTCP_ACK:
     		ack_flag = 1;
     		return;
     	case MTCP_BIOC_EVENT:
     		busy = 1;
     		tuxctl_ioctl_get_button(b, c);
     		busy = 0;
     		return;
     	case MTCP_RESET:
     		tuxctl_ioctl_init(tty);	
     		if(!ack_flag)
 				return ;			
		 	tuxctl_ioctl_set_led(tty, led_status);	
			return; 
		 default: return;
     }

    /*printk("packet : %x %x %x\n", a, b, c); */
}

/******** IMPORTANT NOTE: READ THIS BEFORE IMPLEMENTING THE IOCTLS ************
 *                                                                            *
 * The ioctls should not spend any time waiting for responses to the commands *
 * they send to the controller. The data is sent over the serial line at      *
 * 9600 BAUD. At this rate, a byte takes approximately 1 millisecond to       *
 * transmit; this means that there will be about 9 milliseconds between       *
 * the time you request that the low-level serial driver send the             *
 * 6-byte SET_LEDS packet and the time the 3-byte ACK packet finishes         *
 * arriving. This is far too long a time for a system call to take. The       *
 * ioctls should return immediately with success if their parameters are      *
 * valid.                                                                     *
 *                                                                            *
 ******************************************************************************/
int 
tuxctl_ioctl (struct tty_struct* tty, struct file* file, 
	      unsigned cmd, unsigned long arg)
{
    switch (cmd) {
	case TUX_INIT:	return  tuxctl_ioctl_init(tty);
	case TUX_BUTTONS:	return  tuxctl_ioctl_buttons(tty, arg);
	case TUX_SET_LED:	return  tuxctl_ioctl_set_led (tty, arg);
	case TUX_LED_ACK:	return 0;
	case TUX_LED_REQUEST:	return 0;
	case TUX_READ_LED:   return 0;
	default:
	    return -EINVAL;
    }
}

/*********************implementation of local functions*************************/


/*tuxctl_ioctl_init
 *DESCRIPTION: Initialize TUX
 *INPUT: tty - a pointer to a tty_struct type argument, used for tuxctl_ldisc_put
 *OUTPUT: None
 *RETURN: 0
 *SIDE EFFECT: Initialize led_status and button_status
 */
 int tuxctl_ioctl_init(struct tty_struct* tty)
 {
 	unsigned char value[2];

 	ack_flag = 0;
 	
	value[0] = MTCP_BIOC_ON;				//Enable Button interrupt-on-change.
 	value[1] = MTCP_LED_USR;				//Put the LED display into user-mode.

 	tuxctl_ldisc_put(tty, &value[0], 1);
 	tuxctl_ldisc_put(tty, &value[1], 1);

 	
 	led_status = 0;							//initialize led_status and buttons
 	button_status.buttons = 0xFF;
 	button_status.buttons_lock = SPIN_LOCK_UNLOCKED;

 	return 0;
 }


/*tuxctl_ioctl_set_led
 *DESCRIPTION: Display arg to LED on TUX
 *INPUT: tty - a pointer to a tty_struct type argument, used for tuxctl_ldisc_put
 *       arg - [15:0] - a number whose hexadecimal value is to be 
 *			   [19:16] - which LED’s should be turned on. 
 *			   [27:24] - whether the corresponding decimal points should be turned on. 
 *OUTPUT: None
 *RETURN: 0 
 *SIDE EFFECT: Same as description
 */

 int tuxctl_ioctl_set_led (struct tty_struct* tty, unsigned long arg)
 {
 	unsigned char display_value[4];
 	unsigned char led;
 	unsigned char dec;
 	unsigned int  i;
 	unsigned long bitmask; 
 	unsigned char buffer[6];

 	if(!ack_flag)
 		return -1;
 	
 	
 
 	bitmask = 0x000F;					//handle information from arg
 	for(i = 0; i < 4; ++i, bitmask <<= 4){
 		display_value[i] = (bitmask & arg) >> (4*i);
 	}

 	led = (arg & (0x0F << 16)) >> 16;
 	dec = (arg & (0x0F << 24)) >> 24;

 	buffer[0] = MTCP_LED_USR;
 	tuxctl_ldisc_put(tty, &buffer[0], 1);	//send data to tux

 	buffer[0] = MTCP_LED_SET;
 	buffer[1] = 0x0F;

 	bitmask = 0x01;
 	for(i = 0; i < 4; ++i, bitmask <<= 1){
 		if(led & bitmask){
 			display_value[i] = seven_segment[display_value[i]];
 			if(dec & bitmask){
 				display_value[i] |= 0x10;
 			}
 			buffer[2 + i] = display_value[i];
 		}
 		else{
 			buffer[2 + i] = 0x0;
 		}
 	}
 	led_status = arg;				//save to led_status

 	tuxctl_ldisc_put(tty, buffer, 6);		//send data to tux

 	ack_flag = 0;
	return 0;
 }



/*tuxctl_ioctl_buttons
 *DESCRIPTION: put the status of button to arg
 *INPUT: tty - a pointer to a tty_struct type argument
 		 arg - wstatus of button
 *OUTPUT: None
 *RETURN: 0 if success else -1
 *SIDE EFFECT: None
 */
int tuxctl_ioctl_buttons(struct tty_struct* tty, unsigned long arg)
{
	unsigned long flags;
	unsigned long *temp;	//pointer to buttons
	int a;
	temp = &(button_status.buttons);

	spin_lock_irqsave(&(button_status.buttons_lock), flags); 			//beginning of spin lock
	a = copy_to_user((void *)arg, (void *)temp, sizeof(long));                //copy data to user
	spin_unlock_irqrestore(&(button_status.buttons_lock), flags);

	if (a > 0)
		return -1;
	else
		return 0;
}

/*tuxtl_handle_get_button
 *DESCRIPTION: get the status of button and save in buttons
 *INPUT: b - XXXXCBAS 
 *		 c - XXXXRDLU
 *OUPUT: None
 *RETURN: 0
 *SIDE EFFECT: change the lowest byte of global variable button to RLDUCBAS
 */
int tuxctl_ioctl_get_button(unsigned b, unsigned c)
{
	unsigned long flags;
	b = ~b;
	c = ~c;

	spin_lock_irqsave(&(button_status.buttons_lock), flags);
	button_status.buttons = ~((b & 0x0F) | ((c & 0x0F) << 4));                //organize b and c to buttons
	spin_unlock_irqrestore(&(button_status.buttons_lock), flags);	
	return 0;
}
