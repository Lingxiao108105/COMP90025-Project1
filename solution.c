#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0

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

// data for sequential algorithm
typedef struct sequential_data{
    int s,t;      // from s to t
    int node_number;
    int **adjacent_matrix;
    int **flows;  // preflows
    int *excess;  // excess
    int *d;       // the labels
    Queue *active_nodes;
}Sequential_Data;



// return the minimum of two
int min(int a, int b);

//===========================================================
//adjacent matrix
/**
 * read adjacent matrix from input
 * return adjacent matrix
 */
int **read_adjacent_matrix(int *node_number);
/**
 * create empty adjacent matrix
 */
int **create_adjacent_matrix(int node_number);
//free the adjacent matrix
void free_adjacent_matrix(int node_number, int** adjacent_matrix);
//print the adjacent matrix to stdout
void print_adjacent_matrix(int node_number, int** adjacent_matrix);

//===========================================================
//queue

// create an empty priority queue
Queue *create_queue();
// create a new node
Queue_Node *new_node(int node);
// whether the queue is empty
int isEmpty(Queue *queue);
// free a priority queue and data inside
void free_queue(Queue *queue);
// push a node to the tail of queue
void push(Queue *queue, int node);
// pop the first element and return node
int pop(Queue *queue);

//===========================================================
//sequential algorithm

// init the sequential data
Sequential_Data *sequential_init(int node_number, int **adjacent_matrix, int s, int t);
// free the Sequential_Data(exclude adjacent matrix)
void free_sequential_data(Sequential_Data *data);
// try to push flow (u is not s or t)
int sequential_push(Sequential_Data *data, int u);
// d(u) = 1 + min d(v)
void sequential_relabel(Sequential_Data *data, int u);
// sequential version of pushrelabel algorithm
void sequential_pushrelabel(int node_number, int **adjacent_matrix, int s, int t);


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
    sequential_pushrelabel(node_number,adjacent_matrix,3,0);
    free_adjacent_matrix(node_number,adjacent_matrix);

}

// return the minimum of two
int min(int a, int b){
    if(a < b){
        return a;
    }
    return b;
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

    adjacent_matrix = create_adjacent_matrix(*node_number);

    //read the edges
    while(scanf("%d %d %d\n", &u,&v,&capacity) == 3){
        adjacent_matrix[u][v] = capacity;
    }

    return adjacent_matrix;

}

/**
 * create empty adjacent matrix
 */
int **create_adjacent_matrix(int node_number){
    int i;
    int **adjacent_matrix;

    //allocate the adjacent matrix
    adjacent_matrix = (int **)(malloc(sizeof(int*) * (node_number)));
    for(i=0;i<node_number;i++){
        adjacent_matrix[i] = (int *)(calloc(node_number,sizeof(int)));
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

// whether the queue is empty
int isEmpty(Queue *queue){
    return queue->length == 0;
}

// free a priority queue and data inside
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
        return;
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
        printf("ERROR: try to pop from an empty queue！");
        return -1;
    }

    Queue_Node *temp = queue->head;
    node = temp->node;
    queue->head = queue->head->next;
    (queue->length)--;
    free(temp);
    return node;
}

//init the sequential data
Sequential_Data *sequential_init(int node_number, int **adjacent_matrix, int s, int t){
    int i,j;

    // create data
    Sequential_Data *temp = (Sequential_Data *)malloc(sizeof(Sequential_Data));
    temp->adjacent_matrix = adjacent_matrix;
    temp->node_number = node_number;
    temp->s = s;
    temp->t = t;
    // malloc space
    temp->d = (int *)calloc(node_number, sizeof(int)); 
    temp->excess = (int *)calloc(node_number, sizeof(int));
    temp->flows = create_adjacent_matrix(node_number);
    temp->active_nodes = create_queue();

    // saturating all the edges sv coming out the source s
    temp->d[s] = node_number;
    for(i=0;i<node_number;i++){
        if(i == s){
            continue;
        }
        if(adjacent_matrix[s][i] != 0){
            // saturating the capacity
            temp->flows[s][i] = adjacent_matrix[s][i];
            // excess increase by capacity of sv
            temp->excess[i] = adjacent_matrix[s][i];
            // add residual flow
            temp->flows[i][s] = -(temp->flows[s][i]);
            // node will become active
            if(i != t){
                push(temp->active_nodes,i);
            }
        }
    }

    return temp;

}

// free the Sequential_Data(exclude adjacent matrix)
void free_sequential_data(Sequential_Data *data){

    free_adjacent_matrix(data->node_number,data->flows);
    free(data->excess);
    free(data->d);
    free_queue(data->active_nodes);
    free(data);

}

// try to push flow (u is not s or t)
int sequential_push(Sequential_Data *data, int u){
    int v,flow;
    int push_flag = 0; //whether the node v change to an active node
    for(v=0;v<data->node_number;v++){
        //push if v with admissible arc uv
        if(data->d[u] == data->d[v] + 1 && 
           (data->adjacent_matrix[u][v] - data->flows[u][v]) > 0){

            // send flow = min(c(uv),e(u))
            flow = min(data->adjacent_matrix[u][v] - data->flows[u][v],data->excess[u]);

            // excess e(u) to fall by send flow
            data->excess[u] -= flow;

            // excess e(v) to increase by send flow(but exclude s and t)
            if(v != data->s && v != data-> t){
                //check whether the node v will become active
                if(data->excess[v] == 0){
                    push_flag = 1;
                }else{
                    push_flag = 0;
                }
                data->excess[v] += flow;
                //push to active queue
                if(push_flag){
                    push(data->active_nodes,v);
                }
            }

            // add residual flow
            data->flows[u][v] += flow;
            data->flows[v][u] -= flow;
            return TRUE;
        }
    }
    return FALSE;
}

// d(u) = 1 + min d(v)
void sequential_relabel(Sequential_Data *data, int u){ 
  int min_height = INT_MAX; 
  int v;

  for (v=0;v<data->node_number;v++){ 
    if ((data->adjacent_matrix[u][v] - data->flows[u][v]) > 0) { 
      min_height = min(min_height, data->d[v]); 
    } 
  }
  data->d[u] = 1 + min_height;
  push(data->active_nodes,u);

}

// sequential version of pushrelabel algorithm
void sequential_pushrelabel(int node_number, int **adjacent_matrix, int s, int t){
    int u;

    Sequential_Data *data = sequential_init(node_number,adjacent_matrix,s,t);

    while(!isEmpty(data->active_nodes)){
        u = pop(data->active_nodes);

        while(data->excess[u] > 0){
            if(!sequential_push(data,u)){
                sequential_relabel(data,u);
            }
        }
    }

    printf("maxflow is %d",data->excess[s]);
    free_sequential_data(data);
}

