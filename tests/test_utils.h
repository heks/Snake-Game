#include <stdbool.h>

#include "game.h"

bool approx_equal (float actual, float expected, float tolerance);
void test_result_pointer(char * label, void * pointer);
void test_bool (char * label, bool actual, bool expected);
void test_int (char * label, int actual, int expected);
void test_float (char * label, float action, float expected, float tolerance);
void test_board (char * label, board * actual, board * expected);
void custom_test (char * label, bool result);

typedef void test_function();
int run_tests(test_function * functions[]);
