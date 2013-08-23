#include <stdio.h>
#include "dataframe.h"

int main(int argc, char **argv)
{
    FILE *infile;
    char *str;
    int **df=NULL;
    if (!argv) {
        printf("please input file name\n");
        exit(1);
    }
    infile = fopen(argv[1], "r");
    if (!infile) {
        printf("can't open file\n");
    }
    while(strlen(fgets(str, 255, infile)) > 0) {
        df[i] = 
        

    return 0;
}

