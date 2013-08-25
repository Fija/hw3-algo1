#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arrlib.h"

int kargerMinCut(int **origin_graph)
{
    const DATAFRAME_METHOD df = Dataframe;
    int **graph;
    int node1=0, node2=0, i, j, k, m, nrow, min_cut;
    size_t nonempty_row;
    graph = df.init("copy", origin_graph);
    //df.print(graph);
    min_cut = graph[-1][1]-1;
    nonempty_row = df.neptr(graph);
    for (m = 0; m < 220000; m++) {
        srand((unsigned int)(m*time(0)));
        while(nonempty_row > 2) {

            /* Pick a random edge */
            k = rand()%(df.elmcnt(graph)-graph[-1][1]);
            //printf("%d\n", k);
            k += 1;
            node1 = 0;
            while (1) {
                nrow = graph[node1][-1]-1;
                if (k > nrow) {
                    k -= nrow;
                    node1 += 1;
                }else break;
            }
            node2 = graph[node1][k]-1;
            //printf("Contract %d to %d\n", node2+1, node1+1);
                

            /* Contract the edge */
            for (i = 0; i < graph[-1][1]; i++) {
                for (j = 1; j < graph[i][-1]; j++) {
                    /* If any vertex connect to node2, combine it to node1 */
                    if (graph[i][j] == node2+1) {
                        graph[i][j] = node1+1;
                    }
                }
            }
            df.append(graph, "arr", node1, graph[node2][-1]-1, graph[node2]+1);
            //df.print(graph);
            df.remove(graph, "arr", node2, 1, graph[node2][-1]-1);
            //df.print(graph);
            nonempty_row -= 1;
            //df.print(graph);


            //printf("Remove self loop of vertex %d\n", node1+1);
            /* Remove self loop */
            for(j = 1; j < graph[node1][-1]; ) {
                if(graph[node1][j] == node1+1) {
                    df.remove(graph, "elm", node1, j);
                }else j++;
            }
            //df.print(graph);
        }
        //printf("%d\n", graph[node1][-1]-1);
        //df.print(graph);
        if (graph[node1][-1]-1 < min_cut) {
            min_cut = graph[node1][-1]-1;
        }
        df.copy(graph, origin_graph);
        nonempty_row = df.neptr(graph);
    }

    /* Return the min cut */
    //df.print(graph);
    df.free(graph);
    return min_cut;
        
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
        exit(1);
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
            if (token != NULL && strcmp("\r\n", token)) {
                graph[-1][2] += 1;
            }
            while (token != NULL && strcmp("\r\n", token)) {
                graph[i][j] = atoi(token);
                graph[i][-1] += 1;
                graph[-1][3] += 1;
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

