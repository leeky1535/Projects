#ifndef TESTS_H
#define TESTS_H


#define FBUFFER_LEN 1024
#define PASS 1
#define FAIL 0

#define TEST_DF 1
#define TEST_RFBI 2
#define TEST_RFBN 3

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

// test launcher
void launch_tests();
void test_display_files();
void test_read_file_by_idx();
void test_read_file_by_name();

#endif /* TESTS_H */
