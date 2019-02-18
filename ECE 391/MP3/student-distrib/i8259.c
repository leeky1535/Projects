/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {

	outb(ICW1, MASTER_8259_PORT);
	outb(ICW1, SLAVE_8259_PORT);

	outb(ICW2_MASTER, MASTER_8259_PORT_DATA);
	outb(ICW2_SLAVE, SLAVE_8259_PORT_DATA);

	outb(ICW3_MASTER, MASTER_8259_PORT_DATA);
	outb(ICW3_SLAVE, SLAVE_8259_PORT_DATA);

	outb(ICW4, MASTER_8259_PORT_DATA);
	outb(ICW4, SLAVE_8259_PORT_DATA);

	master_mask = 0xff;                    //Initialize with all interrupts disabled
	slave_mask = 0xff;

	outb(master_mask, MASTER_8259_PORT_DATA);              //Mask all interrupts on PIC
	outb(slave_mask, SLAVE_8259_PORT_DATA);

	enable_irq(SLAVE_IRQ_ON_MASTER);                       //Enable slave on IRQ 3 of master PIC

}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
	uint8_t mask; 							//Mask for modifying master/slave masks
	
	if (irq_num > 15) 						//Return if irq_num is out of bounds
 		return;

 	if (irq_num < 8){                       //IRQ on master PIC
 		mask = 1 << irq_num;
 		master_mask = master_mask & ~mask; 		//Apply to master mask
 		outb(master_mask, MASTER_8259_PORT_DATA);
 	}
 	else{									//IRQ on slave PIC
 		mask = 1 << (irq_num - 8); 					//Subtract 8 since it's on slave
 		slave_mask = slave_mask & ~mask; 		//Apply to slave mask
 		outb(slave_mask, SLAVE_8259_PORT_DATA);
 	}

}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
	uint8_t mask; 							//Mask for modifying master/slave masks
	
	if (irq_num > 15) 						//Return if irq_num is out of bounds
 		return;

 	if (irq_num < 8){                        //IRQ on master PIC
 		mask = 1 << irq_num;
 		master_mask = master_mask | mask; 		//Apply to master mask
 		outb(master_mask, MASTER_8259_PORT_DATA);
 	}
 	else{									//IRQ on slave PIC
 		mask = 1 << (irq_num - 8);  			//Subtract 8 since it's on slave
 		slave_mask = slave_mask | mask; 		//Apply to slave mask
 		outb(slave_mask, SLAVE_8259_PORT_DATA);
 	}

}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
	if (irq_num > 15) 						//Invalid index
		return;

	if (irq_num >= 8) 						//We must send to slave and master
	{
		outb((irq_num - 8) | EOI, SLAVE_8259_PORT);
		outb(SLAVE_IRQ_ON_MASTER | EOI, MASTER_8259_PORT);
	}
	else
		outb(irq_num | EOI, MASTER_8259_PORT); 				//Just send to master

}
