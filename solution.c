#include <stdio.h>
#include <stdlib.h>

#define NUM_CONFIGURATION 1


/**
 * read adjacent matrix from input
 */
int **read_adjacent_matrix(int *node_number);
//print the adjacent matrix to stdout
void print_adjacent_matrix(int node_number, int** adjacent_matrix);

// read
int main(int argc, char * argv[]){

    int node_number;
    int **adjacent_matrix;
    
    if(argc != NUM_CONFIGURATION + 1){
        perror("Not enough configuration!");
        exit(1);
    }

    //scan the input
    adjacent_matrix = read_adjacent_matrix(&node_number);

}

/**
 * read adjacent matrix from input
 * return adjacent matrix
 */
int **read_adjacent_matrix(int *node_number){
    // store input edge u->v with capactiy
    int u,v,capacity;
    int **adjacent_matrix;
    int i;

    //scan the number of node
    scanf("%d\n", node_number);

    //allocate the adjacent matrix
    adjacent_matrix = (int **)(malloc(sizeof(int*) * (*node_number)));
    for(i=0;i<*node_number;i++){
        adjacent_matrix[i] = (int *)(calloc(0,sizeof(int) * (*node_number)));
    }

    //read the edges
    while(scanf("%d %d %d\n", &u,&v,&capacity) == 3){
        adjacent_matrix[u][v] = capacity;
    }

    return adjacent_matrix;

}

//print the adjacent matrix to stdout
void print_adjacent_matrix(int node_number, int** adjacent_matrix){

    int i,j;
    for(i=0;i<node_number;i++){
        for(j=0;j<node_number;j++){
            printf("%d ",adjacent_matrix[i][j]);
        }
        printf("\n");
    }

}
