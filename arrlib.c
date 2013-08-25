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
static int dataframe_neptr(int **df);
static int dataframe_elmcnt(int **df);
static void dataframe_remove(int **df, char *arg, ...);
static void dataframe_append(int **df, char *arg, ...);
static void dataframe_copy(int **dest, int **src);
    
/* global definition of Array */
const DATAFRAME_METHOD Dataframe = {
    .init = dataframe_init,
    .free = dataframe_free,
    .print = dataframe_print,
    .neptr = dataframe_neptr,
    .elmcnt = dataframe_elmcnt,
    .append = dataframe_append,
    .remove = dataframe_remove,
    .copy = dataframe_copy,
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
        df[0] = (int *)malloc(4 * sizeof(int));
        df[0][0] = dim_one; /* n of row allocated */
        df[0][1] = 0;       /* n of row meaningful */
        df[0][2] = 0;       /* n of nonempty row */
        df[0][3] = 0;       /* n of total element */
        for (i = 1; i < dim_one+1; i++) {
            df[i] = (int *)malloc((dim_two+2)*sizeof(int)) + 2;
            assert (df[i]);
            df[i][-2] = dim_two;
            df[i][-1] = 0;
        }
        return df+1;
    }else if (0 == strcmp(arg, "copy")) {
        const int **src = va_arg(arg_ptr, const int **);
        int **dest = (int **)malloc((src[-1][0]+1)*sizeof(int *)) + 1;
        dest[-1] = (int *)malloc(4*sizeof(int));
        dest[-1][0] = src[-1][1];
        memmove(dest[-1]+1, src[-1]+1, 3*sizeof(int));
        for (i = 0; i < src[-1][1]; i++) {
            dest[i] = (int *)malloc((src[i][-1]+2)*sizeof(int)) + 2;
            dest[i][-2] = src[i][-1];
            memmove(dest[i]-1, src[i]-1, (src[i][-1]+1)*sizeof(int));
        }
        return dest;
    }else {
        printf("Unknown parameter\n");
        return df;
    }

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
    printf("Dataframe row = %d, rowsize = %d, nonemptyrow = %d, "\
           "total element = %d\n", df[-1][1], df[-1][0], df[-1][2],\
           df[-1][3]);  
    printf("**************************************\n");
    for (i = 0; i < df[-1][1]; i++) {
        printf("[");
        for (j = 0; j < df[i][-1]; j++) printf("%d ", df[i][j]);
        printf("], count = %d, length = %d \n", df[i][-1], df[i][-2]);
    }
    printf("**************************************\n");
}

static int dataframe_neptr(int **df)
{
    return df[-1][2];
}

static int dataframe_elmcnt(int **df)
{
    return df[-1][3];
}

static void dataframe_append(int **df, char *arg, ...)
{
    va_list arg_ptr;
    size_t nrow, nelm;
    const int *arr;
    int *tmp_ptr;
    va_start(arg_ptr, arg);
    if (0 == strcmp(arg, "arr")) {
        nrow = va_arg(arg_ptr, size_t);
        nelm = va_arg(arg_ptr, size_t);
        arr = va_arg(arg_ptr, const int *);
        if (df[nrow][-2] < df[nrow][-1] + nelm) {
            tmp_ptr = (int *)realloc(df[nrow]-2,\
                                    (df[nrow][-1]+nelm+2)*sizeof(int));
            assert(tmp_ptr);
            df[nrow] = tmp_ptr + 2;
            df[nrow][-2] = df[nrow][-1] + nelm;
        }
        memmove(df[nrow]+df[nrow][-1], arr, nelm*sizeof(int));
        df[nrow][-1] += nelm;
        if (df[nrow][-1] == 0) df[-1][2] += 1;
        df[-1][3] += nelm;
    }
}

static void dataframe_remove(int **df, char *arg, ...)
{
    va_list arg_ptr;
    size_t nrow, ncol, start, end;
    va_start(arg_ptr, arg);
    if (0 == strcmp(arg, "arr")) {
        nrow = va_arg(arg_ptr, size_t);
        start = va_arg(arg_ptr, size_t);
        end = va_arg(arg_ptr, size_t);
        assert(nrow < df[-1][1]);
        assert(start >= 0 && start < df[nrow][-1]);
        assert(start <= end && end < df[nrow][-1]);
        if (end < df[nrow][-1]-1) {
            memmove(df[nrow]+start, df[nrow]+end+1,\
                    (df[nrow][-1]-end)*sizeof(int));
        }
        df[nrow][-1] -= end - start + 1;
        df[-1][3] -= end - start + 1; 
        if (df[nrow][-1] == 0) df[-1][2] += 1;
    }else if (0 == strcmp(arg, "elm")) {
        nrow = va_arg(arg_ptr, size_t);
        ncol = va_arg(arg_ptr, size_t);
        assert(nrow < df[-1][1]);
        assert(ncol < df[nrow][-1]);
        if (ncol < df[nrow][-1]-1) {
            memmove(df[nrow]+ncol, df[nrow]+ncol+1,\
                    (df[nrow][-1]-ncol-1)*sizeof(int));
        }
        df[nrow][-1] -= 1;
        df[-1][3] -=1;
        if (df[nrow][-1] == 0 ) df[-1][2] += 1;
    }
}

static void dataframe_copy(int **dest, int **src)
{
    int i;
    if (dest[-1][0] < src[-1][1]) {
       dest = (int **)realloc(dest-1, (src[-1][1]+1)*sizeof(int *)) + 1;
       assert(dest);
       dest[-1][0] = src[-1][1];
    }
    memmove(dest[-1]+1, src[-1]+1, 3*sizeof(int));
    for (i = 0; i < src[-1][1]; i++) {
        if (dest[i][-2] < src[i][-1]) {
            dest[i] = (int *)realloc(dest[i]-2, (src[i][-1]+2)) + 2;
            assert(dest[i]);
            dest[i][-2] = src[i][-1];
        }
        memmove(dest[i]-1, src[i]-1, (src[i][-1]+1)*sizeof(int));
    }
}



    




