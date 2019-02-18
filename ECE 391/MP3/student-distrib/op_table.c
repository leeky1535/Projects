#include "filesys.h"
#include "rtc.h"
#include "op_table.h"

op_table rtc_op_table = {rtc_read, rtc_write, rtc_open, rtc_close};
op_table directory_op_table = {read_directory, no_function, open_directory, close_directory};
op_table data_op_table = {read_data_file, no_function, open_data_file, close_data_file};
op_table std_in_ops = {terminal_read, no_function, terminal_open, terminal_close};
op_table std_out_ops = {no_function, terminal_write, terminal_open, terminal_close};
op_table no_ops = {no_function, no_function, no_function, no_function};

op_table* op_table_table[FTYPE_N] = {&rtc_op_table, &directory_op_table, &data_op_table};

int32_t no_function()	
{
    return -1;
}
