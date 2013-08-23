#include "arrlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/**************************************************
 * Class of Array
 * Description: 1-dim int array
 * Data type: int *
 * Methods: Array.method
 * Usage: initialization needed before using any method
 ***************************************************/

/* Declaration of methods */
static int *array_init(char *arg, ...);
static void array_free(int *arr);
static void array_print(int *arr);

/* global definition of Array */
const ARRAY_METHOD Array = {
    .init = array_init,
    .free = array_free,
    .print = array_print,
};

/* Implementation of methods */

static int *array_init(char *arg, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, arg);
    int *arr = NULL;
    if (0 == strcmp(arg, "raw")) {
        size_t len = va_arg(arg_ptr, size_t);
        arr = (int *)malloc((len+2) * sizeof(int));
        assert (arr);
        arr[0] = len;
        arr[1] = 0;
        return arr+2;
    }else if (0 == strcmp(arg, "val")) {
        int i;
        size_t len = va_arg(arg_ptr, size_t);
        size_t val = va_arg(arg_ptr, size_t);
        arr = (int *)malloc((len+2) * sizeof(int));
        assert (arr);
        arr[0] = len;
        for (i = 0; i < len; i++) arr[i+2] = val;
        arr[1] = len;
        return arr+2;
    }else {
        printf("Arguments unknown.\n");
        return arr;
    }
}


static void array_free(int *arr)
{
    free(arr-2);
    return;
}

static void array_print(int *arr)
{
    int i;
    printf("[");
    for (i = 0; i < arr[-1]; i++) printf("%d ", arr[i]);
    printf("\b]\n");
}





/**************************************************
 * Class of Dataframe
 * Description: 2-dim int array
 * Data type: int **
 * Methods: Dataframe.method
 * Usage: initialization needed before using any method
 ***************************************************/

/* Declaration of methods */
static int **dataframe_init(char *arg, ...);
static void dataframe_free(int **df);
static void dataframe_print(int **df);

/* global definition of Array */
const DATAFRAME_METHOD Dataframe = {
    .init = dataframe_init,
    .free = dataframe_free,
    .print = dataframe_print,
};

/* Implementation of methods */

static int **dataframe_init(char *arg, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, arg);
    int **df = NULL;
    int i;
    if (0 == strcmp(arg, "raw")) {
        size_t dim_one = va_arg(arg_ptr, size_t);
        size_t dim_two = va_arg(arg_ptr, size_t);
        df = (int **)malloc((dim_one+1)*sizeof(int *));
        assert(df);
        df[0] = (int *)malloc(2 * sizeof(int));
        df[0][0] = dim_one;
        df[0][1] = 0;
        for(i = 1; i < dim_one+1; i++) {
            df[i] = (int *)malloc((dim_two+2)*sizeof(int)) + 2;
            assert (df[i]);
            df[i][-2] = dim_two;
            df[i][-1] = 0;
        }
    }
    return df+1;
}


static void dataframe_free(int **df)
{
    int i, dim_one = df[-1][0];
    free(df[-1]);
    for (i = 0; i < dim_one; i++) {
        free(df[i]-2);
    }
    free(df-1);
    return;
}

static void dataframe_print(int **df)
{
    int i, j;
    printf("Dataframe row = %d, rowsize = %d.\n", df[-1][1], df[-1][0]);  
    printf("**************************************\n");
    for (i = 0; i < df[-1][1]; i++) {
        printf("[");
        for (j = 0; j < df[i][-1]; j++) printf("%d ", df[i][j]);
        printf("], count = %d, length = %d \n", df[i][-1], df[i][-2]);
    }
    printf("**************************************\n");
}
