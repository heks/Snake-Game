/*! Test utils module
    
    Provides various functions do the actual vs. expected value comparison
    for test modules.  Also provides a function that runs a lists of test
    and prints the result.
*/

#include "test_utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int tests_passed;
static int tests_run;

void randomize_stack()
{
    int stack_space[1024];
    int index;
    for (index = 0; index < 1024; index++)
    {
        stack_space[index] = random();
    }
}

bool approx_equal(float actual, float expected, float max_error)
{
    if (actual < max_error)
    {
        return fabs(expected - actual) < max_error;
    }
    else
    {
        float relative_error = fabs(expected - actual) / fabs(actual);
        return relative_error < max_error;
    }                                                              
}

void test_result_pointer(char * label, void * pointer)
{
    if (pointer != NULL)
    {
        printf("Pass: %s returns non-null pointer\n", label);
        tests_passed++;
    }
    else
    {
        printf("Fail: %s returns NULL\n", label);
    }
    tests_run++;
}

void test_int(char * label, int actual, int expected)
{

    if (expected == actual)
    {
        printf("Pass: %s: %d = %d\n", label, expected, actual);
        tests_passed++;
    }
    else
    {
        printf("Fail: %s: expected %d, got %d\n",
               label, expected, actual);
    }
    tests_run++;
}

void test_float(char * label, float actual, float expected, float tolerance)
{
    if (approx_equal(expected, actual, tolerance))
    {
        printf("Pass: %s: %f ~= %f\n", label, expected, actual);
        tests_passed++;
    }
    else
    {
        printf("Fail: %s: expected %f, got %f\n", label, expected, actual);
    }
    tests_run++;
}

void print_board(board * cur_board)
{
    int row, col;
    cell * cell = cur_board->cells;
    for (row = 0; row < cur_board->rows; row++)
    {
        for (col = 0; col < cur_board->cols; col++)
        {
            printf(" %d", (int)*cell);
            cell++;
        }
        printf("\n");
    }
    printf("\n");
}

void test_board(char * label, board * actual, board * expected)
{
    if (actual->rows == expected->rows && actual->cols == expected->cols &&
        memcmp(actual->cells, expected->cells, expected->rows * expected->cols * sizeof(actual->cells[0])) == 0)
    {
        printf("Pass: %s: board result matches expected\n", label);
        tests_passed++;
    }
    else
    {
        printf("Fail: %s: expected board:\n", label);
        print_board(expected);
        printf("Actual board:\n");
        print_board(actual);
    }
    tests_run++;
}

void custom_test(char * label, bool result)
{
    if (result)
    {
        printf("Pass: %s\n", label);
        tests_passed++;
    }
    else
    {
        printf("Fail: %s\n", label);
    }
    tests_run++;
}

int run_tests(test_function * functions[])
{
    tests_passed = tests_run = 0;
    test_function ** function;
    for (function = functions; *function; function++)
    {
        randomize_stack();
        (*function)();
    }
    printf("%d out of %d tests passed\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
