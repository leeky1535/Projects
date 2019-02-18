

#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "page.h"
#include "rtc.h"
#include "keyboard.h"
#include "filesys.h"


static uint32_t current_test;
static uint8_t fbuffer [FBUFFER_LEN]; //Very big buffer of arbitrary length used to store file to display to console. For files bigger than 1k multiple reads will be used

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}

static uint32_t file_disp_idx;

/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

/* Paging Test
 * 
 * Making sure that all allocated values are correct for the arrays for the paging structure.
 * Unnecessary for kernel cases for those are individually hard coded.
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Files: page.h/page.c
 */
//int paging_test()
//{
//	TEST_HEADER;
//	
//	int i;
//
//  int result = PASS;
//	for(i = 1; i < 1024; ++i){
//      //Where 84 is the video memory address
//      if( i != 184){
//         if(page_table[i].present != 0 || page_table[i].rw != 1 || page_table[i].user_supervisor != 1 || page_table[i].page_addr != i){
//            result = FAIL;
//         }
//      }
//      else{
//         if(page_table[i].present == 0){
//           result = FAIL;
//         }
//      }
//   return result; 
//}


/* Checkpoint 2 tests */
/*
 * void run_rtc_test()
 * input: none
 * output: none
 * Function: test by showing incrementing rtc frequency activated from keyboard.c (CTRL+3) 
void run_rtc_test(){
	//check if rate is valid 
	clear();
	update_cursor(0,0);
	int rate, i;
	// For testing up to max of 1024 hz incrementing at power of 2.
	for (rate = 2; rate <= 1024; rate *= 2) {
		printf("rtc_rate at: %d", rate);
		printf("\n");
		rtc_write(1,(uint32_t *)&rate,4);
		for (i = 0; i < 10; i++){
			printf("1");
			rtc_read(1,&rate,4);
		}
		printf("\n");
	}
}
*/

/*
 * void test_read_file_by_name()
 * input: none
 * output: none
 * Function: test for reading file by given name. Name can be changed test is called by CTRL + 2 */
void test_read_file_by_name()
{
	current_test = TEST_RFBN;
	char* fname = "frame0.txt";
	
	dentry_t f_dentry;

	read_dentry_by_name(fname, &f_dentry); //You can change this to change file displayed
	
	if (f_dentry.file_type == FILE_TYPE_FILE) //If there is stuff to display, show it
	{
		uint32_t chars, i, offset = 0;
		while ((chars = read_data_file(0, &fbuffer, FBUFFER_LEN)))
		{
			for (i = 0; i < chars; i++)
				putc(fbuffer[i]);
			offset += chars;
		}
		
		printf("\n"); //Lastly, add a new line after the file
	}
	
	printf ("file_name: %s", fname);
}



/*
 * void test_read_file_by_name()
 * input: none
 * output: none
 * Function: test for reading file by its index positoin within directory, 
 *         test is called by CTRL + 3 index can be cycled by CTRL + 3 as well*/
void test_read_file_by_idx()
{
	if (current_test != TEST_RFBI)
		file_disp_idx = 0;
	else{
		file_disp_idx++; //TODO: Limit?
	}
	current_test = TEST_RFBI;
	dentry_t f_dentry;

	read_dentry_by_index(file_disp_idx, &f_dentry);


	if (f_dentry.file_type == FILE_TYPE_FILE) //If there is stuff to display, show it
	{
		uint32_t chars, i, offset = 0;
		while ((chars = read_data_file(0, &fbuffer, FBUFFER_LEN)))
		{
			for (i = 0; i < chars; i++)
				putc(fbuffer[i]);
			offset += chars;
		}

		printf("\n"); //Lastly, add a new line after the file
	}
	//Hack since string might not be null terminated
	char fname_buffer[MAX_FNAME_LENGTH + 1]; //We use the extra character to ensure the string is null terminated
	fname_buffer[MAX_FNAME_LENGTH] = '\0';
	memcpy(&fname_buffer, &(f_dentry.fname), MAX_FNAME_LENGTH);

	printf ("file_name: %s", fname_buffer);
}

/*
 * void test_display_files()
 * input: none
 * output: none
 * Function: test for reading directory and list of files within it test is called by CTRL + 1 
void test_display_files()
{
	current_test = TEST_DF;
	dentry_t dir_dentry;
	read_dentry_by_name(".", &dir_dentry);

	char fname_buffer[MAX_FNAME_LENGTH + 1]; //We use the extra character to ensure the string is null terminated
	fname_buffer[MAX_FNAME_LENGTH] = '\0';

	int i = 0; //Index in directory of file being read
	dentry_t cur_file_dentry; //Dentry of file which is having details outputted
	// We directly call read_directory, since our 0x80 syscall and jump table isn't a part of this CP
	while (read_directory(&dir_dentry, &fname_buffer, MAX_FNAME_LENGTH))
	{
		read_dentry_by_name(fname_buffer, &cur_file_dentry);
		printf("FILE_NAME: ");
		printf("%s,  TYPE: %u,  ", fname_buffer, cur_file_dentry.file_type);
		int32_t file_len = get_file_length(&cur_file_dentry);
		printf("LENGTH:  %d\n", file_len); 

		i++;
	}
}
*/


/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("paging_test", paging_test());
	//run_rtc_test();
	// launch your tests here
}
