#include <stdio.h>
#include <stdlib.h>

#define NUM_CONFIGURATION 1


// Node of linked list
typedef struct queue_node{
    int node;
    struct queue_node *next;
}Queue_Node;

// Priority Queue
typedef struct queue{
    Queue_Node *head;
    Queue_Node *last;
    unsigned int length;
}Queue;

//===========================================================
//adjacent matrix
/**
 * read adjacent matrix from input
 * return adjacent matrix
 */
int **read_adjacent_matrix(int *node_number);
//free the adjacent matrix
void free_adjacent_matrix(int node_number, int** adjacent_matrix);
//print the adjacent matrix to stdout
void print_adjacent_matrix(int node_number, int** adjacent_matrix);
//===========================================================

//===========================================================
//queue

// create an empty priority queue
Queue *create_queue();
// create a new node
Queue_Node *new_node(int node);
//free a priority queue and data inside
void free_queue(Queue *queue);
// push a node to the tail of queue
void push(Queue *queue, int node);
// pop the first element and return node
int pop(Queue *queue);

//===========================================================



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
    print_adjacent_matrix(node_number,adjacent_matrix);

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
        adjacent_matrix[i] = (int *)(calloc(*node_number,sizeof(int)));
    }

    //read the edges
    while(scanf("%d %d %d\n", &u,&v,&capacity) == 3){
        adjacent_matrix[u][v] = capacity;
    }

    return adjacent_matrix;

}

//free the adjacent matrix
void free_adjacent_matrix(int node_number, int** adjacent_matrix){

    int i;
    for(i=0;i<node_number;i++){
        free(adjacent_matrix[i]);
    }
    free(adjacent_matrix);

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

// create an empty priority queue
Queue *create_queue(){
    Queue *temp = (Queue *)malloc(sizeof(Queue));
    temp->head = NULL;
    temp->last = NULL;
    temp->length = 0;
    return temp;
}

// create a new node
Queue_Node *new_node(int node){
    Queue_Node *temp = (Queue_Node *)malloc(sizeof(Queue_Node));
    temp->node = node;
    temp->next = NULL;
 
    return temp;
}

//free a priority queue and data inside
void free_queue(Queue *queue){
    if(queue == NULL){
        return;
    }
    Queue_Node *curr_node=queue->head;
    Queue_Node *temp_node;
    while(curr_node!=NULL){
        temp_node = curr_node;
        curr_node = curr_node->next;
        free(temp_node);
    }
    free(queue);
}

// push a node to the tail of queue
void push(Queue *queue, int node){

    Queue_Node *Qnode;

    if(queue->last==NULL){
        return NULL;
    }

    //create new queue node
    Qnode = new_node(node);
    //add it to the tail of queue
    queue->last->next = Qnode;
    queue->last = Qnode;
    (queue->length)++;

}

// pop the first element and return node
int pop(Queue *queue){

    int node;

    if(queue->head==NULL){
        return NULL;
    }

    Queue_Node *temp = queue->head;
    node = temp->node;
    queue->head = queue->head->next;
    (queue->length)--;
    free(temp);
    return node;
}


