/* filesys.c - the C part of the filesys
 * vim:ts=4 noexpandtab
 * ECE 391 @UIUC, 2017 Fall NamtangOS
 */

#include "filesys.h"
#include "lib.h"
#include "pcb.h"
#include "rtc.h"
#include "op_table.h"

static boot_block_t* bootblock;
static uint32_t fs_start_addr;

/*
 * init_fs
 * DESCRIPTION: initialize file system
 * INPUT: fs_addr - Address of starting location
 * OUTPUT: None
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void init_fs(uint32_t fs_addr)
{
	fs_start_addr = fs_addr;
	bootblock = (boot_block_t*)fs_addr;
}

int32_t open_directory(const uint8_t* fname)
{
	return 0;
}

int32_t close_directory(int32_t fd)
{
	return 0;
}

int32_t open_data_file(const uint8_t* fname)
{
	return 0;
}

int32_t close_data_file(int32_t fd)
{
	return 0;
}

/*
 * read_data_file
 * DESCRIPTION: read data from file
 * INPUT: fd - file descriptor
 *        buffer - buffer to copy into
 *        nbytes - size of the buffer
 * OUTPUT: data to buffer
 * RETURN VALUE: bytes read to the buffer
 * SIDE EFFECTS: None
 */
int32_t read_data_file(int32_t fd, void* buffer, int32_t nbytes)
{
	int32_t bytes_read = read_data(cur_pcb->fd_table[fd].inode, cur_pcb->fd_table[fd].pos, buffer, nbytes);
	
	cur_pcb->fd_table[fd].pos += bytes_read;
	return bytes_read;

}

/* read_directory
 * DESCRIPTION: read directory info
 * INPUT: fd - file descriptor of directory
 *        buffer - buffer to store file names
 *        nbytes - size of the buffer
 * OUTPUT: file names to buffer
 * RETURN VALUE: number of bytes copied
 * SIDE EFFECT: none
 */
int32_t read_directory(int32_t fd, void* buffer, int32_t nbytes)
{
	uint32_t offset = cur_pcb->fd_table[fd].pos;

	/* no more files to read */
	if (offset >= bootblock->num_dentries)
		return 0;

	/* Load current directory entry */
	dentry_t *cur_file_dentry = &(bootblock->dentries[offset]);

	/* Traverse through file descriptor table */
	cur_pcb->fd_table[fd].pos++;

	/* get file name to buffer */
	nbytes = MIN(nbytes, MAX_FNAME_LENGTH); 
	memcpy(buffer, &(cur_file_dentry->fname), nbytes); //Copy it
	
	nbytes = (int32_t)strlen(cur_file_dentry->fname);
	nbytes = MIN(nbytes, MAX_FNAME_LENGTH); 

	return nbytes;
}

/*
 * read_dentry_by_name
 * DESCRIPTION: read directory entry by name
 * INPUT: fname - file name
 *        dentry - directory entry pointer
 * OUTPUT: none
 * RETURN VALUE: 0 - success, -1 - fail
 * SIDE EFFECT: none 
 */
int32_t read_dentry_by_name(const char* fname, dentry_t* dentry)
{
	/* Get length of the file name */
	int len = strlen(fname);
	/* Boundary Check */
	if (len > MAX_FNAME_LENGTH)
		return -1; 

	/* Search for the corresponding directory */
	int i;
	for (i = 0; i < bootblock->num_dentries; i++)
	{
		dentry_t *cur_dentry = &(bootblock->dentries[i]);
		if (strlen(cur_dentry->fname) == len && strncmp(fname, cur_dentry->fname, len) == 0) //Check for match
		{
			memcpy(dentry, &(bootblock->dentries[i]), DENTRY_SIZE);
			return 0;
		}
	}
	return -1;
}

/*
 * read_dentry_by_index
 * DESCRIPTION: read directory entry by index
 * INPUT: index - corresponding index
 *        dentry - directory entry pointer
 * OUTPUT: none
 * RETURN VALUE: 0 - success, -1 - fail
 * SIDE EFFECT: none 
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
	/* Boundary Check */
	if (index >= bootblock->num_dentries)
		return -1;

	/* Get directory entry from kernel memory */
	memcpy(dentry, &(bootblock->dentries[index]), DENTRY_SIZE);
	
	return 0;
}

/*
 * read_data
 * DESCRIPTION: read the data
 * INPUT: inode_idx - inode index of the file
 *        offset    - offset of the reading location in file
 *        length    - length of the bytes to read
 * OUTPUT: copy data into buffer
 * RETURN VALUE: 0 - success, -1 - fail
 * SIDE EFFECT: none
 */
int32_t read_data(uint32_t inode_idx, uint32_t offset, uint8_t* buf, uint32_t length)
{
	/* Boundary Check */
	if (inode_idx >= bootblock->num_inodes)
		return -1;

	/* Get corresponding inode */
	inode_t* inode = (inode_t*)(fs_start_addr + BLOCK_SIZE * (inode_idx + 1));

	/* NULL check */
	if (offset >= inode->length)
		return 0;

	uint32_t bytes_copied = 0;

	/* get the corresponding data block */
	uint32_t block_idx = offset / BLOCK_SIZE;
	uint32_t block_num = inode->data_blocks[block_idx];

	uint32_t block_offset = offset % BLOCK_SIZE;
	
	length = MIN(length, inode->length - offset);

	while (length != 0)
	{
		/* end of the data block */
		if (BLOCK_SIZE - block_offset > length)
		{
			memcpy(buf, (void*)(fs_start_addr + BLOCK_SIZE * (block_num + bootblock->num_inodes + 1) + block_offset), length); 
			bytes_copied += length;
			return bytes_copied;
		}
		
		/* copy data block into the buffer */
		memcpy(buf, (void*)(fs_start_addr + BLOCK_SIZE * (block_num + bootblock->num_inodes + 1) + block_offset), BLOCK_SIZE - block_offset);
		bytes_copied += (BLOCK_SIZE - block_offset);

		/* Increment buffer, block index, and file offset for traversal */
		buf += (BLOCK_SIZE - block_offset);
		offset += (BLOCK_SIZE - block_offset);
		length -= (BLOCK_SIZE - block_offset);
		block_idx++;
		block_num = inode->data_blocks[block_idx];
		/* reset the offset for rest of the blocks */
		block_offset = 0;
	}
	
	return bytes_copied;
}

/**
 * get_file_length
 * DESCRIPTION: get the file length
 * INPUT: dentry - directory entry pointer
 * OUTPUT: none
 * RETURN VALUE: length of the file
 * SIDE EFFECT: none
 */
int32_t get_file_length(dentry_t* dentry)
{
	/* NULL check */
	if (dentry == NULL)
		return -1;
		
	/* Check file type */
	if (dentry->file_type != FILE_TYPE_FILE)
		return 0;

	/* get corresponding inode of the ifle */
	uint32_t inode_num = dentry->inode_num;

	inode_t* inode = (inode_t*)(fs_start_addr + BLOCK_SIZE * (inode_num + 1));

	/* get the length */
	return (int32_t)(inode->length); 
}

/*
 * get_fd
 * DESCRIPTION: get file descriptor
 * INPUT: none
 * OUTPUT: none
 * RETURN VALUE: fd - file descriptor
 * SIDE EFFECT: none
 */
int32_t get_fd()
{
	int32_t fd;
	asm volatile ("bsf %1,%0"
	: "=r" (fd)
	: "r" (cur_pcb->fdbmp)
	: "cc");
	if (fd >= FILE_DESC_NUM)
		return -1;

	cur_pcb->fdbmp ^= (1 << fd);

	return fd;
}

/*
 * free_fd
 * DESCRIPTION: Free file descriptor
 * INPUT: fd - file descriptor to free
 * OUTPUT: none
 * RETURN VALUE: none
 * SIDE EFFECT: none
 */

void free_fd(int32_t fd)
{
	cur_pcb->fdbmp |= (1 << fd);
}
