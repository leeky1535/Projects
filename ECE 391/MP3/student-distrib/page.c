/* page.c - the C part of the paging
 * vim:ts=4 noexpandtab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */

#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "page.h"

/* 
 * paging_init
 * DESCRIPTION: Initialize paging.
 * INPUT: NONE
 * OUTPUT: NONE
 * RETURN: NONE
 * SIDE EFFECT: Enable interrupt. Page directory and page table set.
 */
void paging_init(){
	int i;
	/* Initialize the page_directory and the  page_table */
	for (i=0; i<TABLE_SIZE; i++){
		/* initialize the video_table */
		video_table[i].present = 0;
		video_table[i].page_addr = i;

		/* initialize the page_table */
		page_table[i].present = 0;
		page_table[i].rw = 1;
		page_table[i].user_supervisor = 1;
		page_table[i].page_addr = i;

		/* initialize the page_directory */
		page_directory[i].present = 0;
		page_directory[i].rw =1;
		page_directory[i].page_addr = i*TABLE_SIZE; 
	}

	/* set up the kernel page */
	page_directory[1].present = 1;
	page_directory[1].rw = 1;
	page_directory[1].user_supervisor = 1;
	page_directory[1].page_size = 1;
	page_directory[1].global = 1;
	page_directory[1].page_addr = (uint32_t)(KERNEL_ADDRESS >> BIT_SHIFT); 

	/* set the first page to be empty */
	page_table[0].present = 0;
	page_table[0].rw = 0;
	page_table[0].user_supervisor = 0;
	
	/* set for video memory */
	page_table[VIDEO_MEM_ADDRE].present = 1;
	video_table[0].page_addr = VIDEO_MEM_ADDRE;
	video_table[0].rw = 1;
	video_table[0].user_supervisor = 1;
	video_table[0].present = 1;
	page_directory[USER_VID_IDX].page_addr = (uint32_t)video_table;
	page_directory[USER_VID_IDX].present = 1;
	page_directory[USER_VID_IDX].user_supervisor = 1;

	
	/* allocate the fisrt page table into the directory table */
	page_directory[0].present = 1;
	page_directory[0].rw = 1;
	page_directory[0].user_supervisor = 1;
	page_directory[0].page_addr = ((uint32_t) (&page_table) >> BIT_SHIFT);

	tlb_flush();

	/* enable paging */
	uint32_t dir_ptr = (uint32_t)&page_directory;
	enable_4MB_paging(dir_ptr);
}

/* 
 * enable_4MB_paging
 * DESCRIPTION: Enable 4Mb paging function.
 				Example from the wiki.osdev.org/Paging
 * INPUT: Address (page_directory address)
 * OUTPUT: NONE
 * RETURN: NONE
 * SIDE EFFECT: NONE
 */

void enable_4MB_paging(uint32_t address)
{
	asm volatile("movl %%eax,%%cr3 \n\
				movl %%cr4,%%eax  \n\
				orl  $0x00000010,%%eax \n\
				movl %%eax,%%cr4       \n   \
				movl %%cr0,%%eax      \n   \
				orl  $0x80000000,%%eax  \n  \
				movl %%eax ,%%cr0 "
	:  
	: "a"(address)
	: "memory", "cc" );
}


/*
 * void map_user_program(int pid)
 * DESCRIPTION: Map the virtual address of a program to the physical address
 * OUTPUT: none
 * RETURN VALUES: 0 on success, -1 if PID is invalid
 * SIDE EFFECTS: 0x800000 is mapped to the memory of the program specified in PID
 */
void map_user_program(uint32_t pid)
{
	page_directory_entry_desc usr_prog;
	usr_prog.present = 1;
	usr_prog.rw = 1;
  	usr_prog.page_size = 1;
   	usr_prog.user_supervisor = 1;
   	usr_prog.page_addr = ((PDE_SIZE_LARGE * (0x2 + pid)) >> BIT_SHIFT);

	page_directory[USER_PD_IDX] = usr_prog;
	
	tlb_flush();
}


/*
 * void flush_tlb()
 * DESCRIPTION: Flush tlb by writing to cr3
 * INPUTS: NONE
 * OUTPUTS: NONE
 * RETURN VALUES: NONE
 * EFFECTS: Flushes TLB
 */
void tlb_flush()
{
  asm volatile (
		"movl %%cr3, %%eax \n\
         movl %%eax, %%cr3"
		:
		:
		: "eax");
}
