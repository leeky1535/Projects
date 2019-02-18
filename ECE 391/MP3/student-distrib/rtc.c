/* rtc.c - the H part of the RTC
 * vim:ts=4 no expand tab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */

#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

/* some global variables for this file scope */
int rate = R_2HZ;
volatile uint8_t rtc_irq_flag;

/* helper function for setting rtc interrupt rate */
void set_rtc_interrupt_rate();


/*
 *  rtc_init
 *  DESCRIPTION: Initialize RTC and enable interrupts
 *  INPUT: none
 *  OUTPUT: none
 *  RETURN VALUE: none
 *  SIDE EFFECT: none 
 */
void rtc_init() {
	// reference: wiki.osdev.org/RTC
	rate &= 0x0F;
	set_rtc_interrupt_rate();
	// Turn on the periodic interrupt
	outb(STATUS_REG_B, INDEX_PORT);			// select register B, and disable NMI
	char prev = inb(CMOS_RW);					// read the current value form register B
	outb(STATUS_REG_B, INDEX_PORT);			// set the index again
	outb(prev | 0x40, CMOS_RW);				//write the previous value ORed with 0x40. This turns on bit 6 of register B

	// enable interrupts
	//enable_irq(IRQ2);
	enable_irq(IRQ8);
	
}

/**
 *  set_rtc_interrupt_rate
 *  DESCRIPTION: helper function for setting the rate to the rtc
 *  INPUT: none
 *  OUTPUT: interrupt rate to rtc
 *  RETURN VALUE: none
 *  SIDE EFFECT: none
 */
void set_rtc_interrupt_rate() {
	outb(STATUS_REG_A, INDEX_PORT);			// set index to register A, disable NMI
	char prevA = inb(CMOS_RW);				// get initial value of register A
	outb(STATUS_REG_A, INDEX_PORT);			// reset index to A
	outb((prevA & 0xF0) | rate, CMOS_RW);	// write only our rate to A. Rate is the bottom 4 bits.
}

/*
 *  rtc_handler
 *  DESCRIPTION: Handle RTC interrupts
 *  INPUT: none
 *  OUTPUT: none
 *  RETURN VALUE: none
 *  SIDE EFFECT: none 
 */
void rtc_handler() {
	//critical section starts
	//cli();
	// test_interrupts();               //for rtc test
	// reference: wiki.osdev.org/RTC
	rtc_irq_flag = 1;
	outb(STATUS_REG_C, INDEX_PORT);  // select register C
	inb(CMOS_RW);					 // throw away contents
	send_eoi(IRQ8);

	//critical section ends
	//sti();
}

/**
 *  rtc_open
 *  DESCRIPTION: open RTC
 *  INPUT: none
 *  OUTPUT: none
 *  RETURN VALUE: 0
 *  SIDE EFFECT: initialize RTC rate and disable the interrupt flag
 */
int32_t rtc_open(const uint8_t* filename) {
	/* initialize the rate and disable flag to block interrupts */
	rate = R_2HZ;
	rtc_irq_flag = 0;
	set_rtc_interrupt_rate();
	return 0;
}

/**
 *  rtc_close
 *  DESCRIPTION: close RTC
 *  INPUT: none
 *  OUTPUT: none
 *  RETURN: 0
 *  SIDE EFFECT: none
 */
int32_t rtc_close(int32_t fd) {
	return 0;
}

/**
 *  rtc_read
 *  DESCRIPTION: enable read from rtc.
 *               set interrupt flag and wait until the flag is cleared
 *  INPUT: none
 *  OUTPUT: none
 *  RETURN VALUE: 0
 *  SIDE EFFECT: none
 */
int32_t rtc_read(int32_t fd, void* buf, int32_t  nbytes) {
	while(rtc_irq_flag == 0){

	}
	rtc_irq_flag = 0;
	return 0;
}

/**
 *  rtc_write
 *  DESCRIPTION: write data to RTC
 *               read interrupt rate from buffer and set the rate accordingly
 *  INPUT: buf - buffer containing the interrupt rate
 *         nbytes - number of bytes of the buffer
 *  OUTPUT: interrupt rate
 *  RETURN VALUE: nbytes : number of bytes written on success, -1 : failure.
 *  SIDE EFFECT: none
 */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes) {
	/* NULL check */
	if (buf == NULL)
		return -1;
	/* only 4 byte integer is accepted */
	if (nbytes != 4)
		return -1;
	
	/* read frequency from buffer */
	uint32_t freq;
	freq = *((uint32_t*)buf);

	/* set the rate to corresponding frequency */
	switch(freq) {
		case 2:
			rate = R_2HZ;
			break;
		case 4:
			rate = R_4HZ;
			break;
		case 8:
			rate = R_8HZ;
			break;
		case 16:
			rate = R_16HZ;
			break;
		case 32:
			rate = R_32HZ;
			break;
		case 64:
			rate = R_64HZ;
			break;
		case 128:
			rate = R_128HZ;
			break;
		case 256:
			rate = R_256HZ;
			break;
		case 512:
			rate = R_512HZ;
			break;
		case 1024:
			rate = R_1024HZ;
			break;
		default:
			/* return -1 for invalid frequency */
			return -1;
	}

	/* disable interrupt.
	 * use cli and sti instead of the flag to prevent confusion with rtc_read.
	 */
	cli();
	/* set the new rate of the rtc */
	set_rtc_interrupt_rate();
	sti();

	return nbytes;
}
