#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arrlib.h"

int kargerMinCut(int **graph)
{
    const DATAFRAME_METHOD df = Dataframe;
    while(df.neptr(graph) > 2) {
        /* Pick a random edge */
        srand((unsigned int)time(0));
        k = rand()%df.elecnt(graph);
        get_edge(graph, k, &node1, &node2);

        /* Contract the edge */
        for (i = 0; i < graph[-1][1]; i++) {
            for (j = 0; j < graph[i][-1]; j++) {
                /* If any vertex connect to node2, combine it to node1 */
                if (graph[i][j] == node2) {
                    graph[i][j] = node1;
                }
            }
        }
        df.append(graph, "arr", nrow, nelm, graph[node2]);
        df.remove(graph, "rval", nrow);

        /* Remove self loop */
        for (j = 0; j < graph[node1][-1]; j++) {
            if (graph[node1][j] == node1) {
                df.remove(graph, "elm", node1, j);
            }
        }
    }


    /* Return the min cut */
    return graph[node1][-1]
        
}
int main(int argc, char **argv)
{
    const DATAFRAME_METHOD df = Dataframe;
    FILE *infile;
    int **graph=NULL;
    int i, j, cut;
    size_t nbytes = 100;
    char *line, *token, *sp;
    if (argv[1]==NULL) {
        printf("please input file name\n");
        exit(1);
    }
    infile = fopen(argv[1], "r");
    if (!infile) {
        printf("can't open file\n");
    }

    graph = df.init("raw", 256, 256);

    /* Input graph from the file with looping */
    i = 0;
    while(1) {
        line = (char *)malloc(nbytes + 1);
        if (EOF == getline(&line, &nbytes, infile)) {
            free(line);
            break;
        }
        else {
            graph[-1][1] += 1;
            j = 0;
            token = strtok_r(line, " \t", &sp);
            while (token != NULL && strcmp("\r\n", token)) {
                graph[i][j] = atoi(token);
                graph[i][-1] += 1;
                token = strtok_r(NULL, " \t", &sp);
                j += 1;
            }
            free(line);
            i += 1;
        }
    }
    //df.print(graph);
    
    /* Calculate the kargerMinCut */
    cut = kargerMinCut(graph);

    printf("%d\n", cut);
    
    df.free(graph);
    fclose(infile);

    return 0;
}

