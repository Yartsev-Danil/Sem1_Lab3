#ifndef TEST_H
#define TEST_H


typedef enum {
	OK,
	ERROR
} Error;


typedef struct {
	Error (*function)();
	const char *name;
} test_function;


#endif // TEST_H
