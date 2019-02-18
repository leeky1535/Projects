/* filesys.h - the H part of the filesys
 * vim:ts=4 noexpandtab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */

#ifndef _FILESYS_H
#define _FILESYS_H

#include "types.h"
#include "keyboard.h"
#include "op_table.h"

#define MAX_FILE_SIZE	4190208 //1023 * 4096

#define DENTRY_SIZE		64
#define BLOCK_SIZE		4096

#define FILE_DESC_NUM  8
#define FILE_TYPE_RTC  0x0
#define FILE_TYPE_DIR  0x1
#define FILE_TYPE_FILE 0x2

#define MAX_FNAME_LENGTH 32


typedef struct __attribute__((packed)) dentry_t
{
	char fname [32];
	uint32_t file_type;
	uint32_t inode_num;
	char reserved [24];
} dentry_t;

typedef struct __attribute__((packed)) inode_t
{
	uint32_t length;
	uint32_t data_blocks [1023];
} inode_t;


typedef struct  __attribute__((packed)) boot_block_t
{
	uint32_t num_dentries;
	uint32_t num_inodes;
	uint32_t num_data_blocks;
	char reserved [52];
	dentry_t dentries [63];
} boot_block_t;

typedef struct __attribute__((packed)) file_desc_t
{
    op_table* fops_table;
    uint32_t inode;
    uint32_t pos;
    uint32_t flags;
} file_desc_t;

void init_fs(uint32_t fs_addr);
int32_t read_dentry_by_name(const char* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode_idx, uint32_t offset, uint8_t* buf, uint32_t length);
int32_t read_directory(int32_t fd, void* buffer, int32_t nbytes);
int32_t read_data_file(int32_t fd, void* buffer, int32_t nbytes);
int32_t get_file_length(dentry_t* dentry);
int32_t get_fd();
void free_fd(int32_t fd);

int32_t open_directory(const uint8_t* fname);
int32_t close_directory(int32_t fd);

int32_t open_data_file(const uint8_t* fname);
int32_t close_data_file(int32_t fd);

#endif //_FILESYS_H
