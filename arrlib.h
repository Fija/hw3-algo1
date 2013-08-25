#include <stdarg.h>

/**************************************************
 * Class of Array
 * Description: 1-dim int array
 * Data type: int *
 * Methods: Array.method
 * Usage: initialization needed before using any method
 ***************************************************/

typedef const struct ARRAY_METHOD {
    int *(*init)(char *arg, ...);
    void (*free)(int *arr);
    void (*print)(int *arr);
} ARRAY_METHOD;

extern const ARRAY_METHOD Array;
    


/**************************************************
 * Class of Dataframe
 * Description: 2-dim int array
 * Data type: int *
 * Methods: Dataframe.method
 * Usage: initialization needed before using any method
 ***************************************************/

typedef const struct DATAFRAME_METHOD {
    int **(*init)(char *arg, ...);
    void (*free)(int **df);
    void (*print)(int **df);
    int (*neptr)(int **df);
    int (*elmcnt)(int **df);
    void (*remove)(int **df, char *arg, ...);
    void (*append)(int **df, char *arg, ...);
    void (*copy)(int **dest, int **src);
} DATAFRAME_METHOD;

extern const DATAFRAME_METHOD Dataframe;
