#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "arrlib.h"

int main(int argc, char **argv)
{
    //const ARRAY_METHOD ar = Array;
    const DATAFRAME_METHOD df = Dataframe;
    FILE *infile;
    int **data=NULL;
    int i, j;
    size_t nbytes = 100;
    char *line, *token;
    if (argv[1]==NULL) {
        printf("please input file name\n");
        exit(1);
    }
    infile = fopen(argv[1], "r");
    if (!infile) {
        printf("can't open file\n");
    }

    data = df.init("raw", 256, 256);

    i = 0;
    while(1) {
        line = (char *)malloc(nbytes + 1);
        if (EOF == getline(&line, &nbytes, infile)) {
            free(line);
            break;
        }
        else {
            data[-1][1] += 1;
            j = 0;
            token = strtok(line, " \t");
            while (token != NULL && strcmp("\r\n", token)) {
                data[i][j] = atoi(token);
                data[i][-1] += 1;
                token = strtok(NULL, " \t");
                j += 1;
            }
            free(line);
            i += 1;
        }
    }
    //df.remove("row", data[-1][1]-1);
    fclose(infile);
    df.print(data);
    df.free(data);

    return 0;
}

