#include "arrlib.h"
#include <stdlib.h>


int main(int argc, char **argv)
{
    const ARRAY_METHOD ar = Array;
    const DATAFRAME_METHOD df = Dataframe;

    int *a, *c = NULL;
    int **b = NULL;
    int i, j, len = 8;

    a = ar.init("raw", len);
    for (i = 0; i < len; i++) {
        a[i] = i;
    };
    ar.print(a);

    c = ar.init("val", len, 19);
    ar.print(c);

    b = df.init("raw", 3, 4);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            b[i][j] = i+j;
        }
    };
    df.print(b);

        
    ar.free(a);
    ar.free(c);
    df.free(b);

    return 0;
}
