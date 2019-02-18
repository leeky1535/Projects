#ifndef _OP_TABLE_H
#define _OP_TABLE_H

#include "types.h"

#define FTYPE_N 3 //Number of file types

typedef struct op_table
{
	int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
	int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
	int32_t (*open)(const uint8_t* filename);
	int32_t (*close)(int32_t fd);
} op_table;

extern op_table rtc_op_table;
extern op_table std_in_ops;
extern op_table std_out_ops;
extern op_table no_ops;

extern op_table* op_table_table[FTYPE_N];

int32_t no_function();

#endif
