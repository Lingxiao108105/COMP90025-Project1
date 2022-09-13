#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>

#define TRUE 1
#define FALSE 0

#define NUM_CONFIGURATION 2


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
int **read_adjacent_matrix(int *node_number, int *edge_number);
/**
 * copy an adjacent matrix
 */
int **copy_adjacent_matrix(int node_number,int** adjacent_matrix);
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
Sequential_Data *sequential_init(int node_number, int **adjacent_matrix);
//init the flow for reuse the data
void sequential_init_flow(Sequential_Data *data, int s, int t);
// free the Sequential_Data(exclude adjacent matrix)
void free_sequential_data(Sequential_Data *data);
// try to push flow (u is not s or t)
int sequential_push(Sequential_Data *data, int u);
// d(u) = 1 + min d(v)
void sequential_relabel(Sequential_Data *data, int u);
// sequential version of pushrelabel algorithm
int sequential_pushrelabel(Sequential_Data *data, int s, int t);
// sequential version of all pair max flow
void sequential_all_pair(int node_number, int **adjacent_matrix);


//===========================================================

// parallel version of all pair max flow
void parallel_all_pair(int node_number, int **adjacent_matrix, int number_thread);


// read
int main(int argc, char * argv[]){

    int node_number,edge_number,is_parallel,number_thread;
    int **adjacent_matrix;
    
    if(argc != NUM_CONFIGURATION + 1){
        perror("Not enough configuration!");
        exit(1);
    }

    is_parallel = atoi(argv[1]);
    number_thread = atoi(argv[2]);

    //scan the input
    adjacent_matrix = read_adjacent_matrix(&node_number,&edge_number);

    //print number of node and edge
    printf("%d %d %d ",node_number,edge_number,number_thread);

    //computation
    if(is_parallel){
        parallel_all_pair(node_number,adjacent_matrix,number_thread);
    }else{
        sequential_all_pair(node_number,adjacent_matrix);
    }
    
    

    //free the matrix
    free_adjacent_matrix(node_number,adjacent_matrix);

    return 0;

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
int **read_adjacent_matrix(int *node_number, int *edge_number){
    // store input edge u->v with capactiy
    int u,v,capacity,current_edge_number = 0;
    int **adjacent_matrix;
    int i;

    //scan the number of node
    scanf("%d\n", node_number);

    adjacent_matrix = create_adjacent_matrix(*node_number);

    //read the edges
    while(scanf("%d %d %d\n", &u,&v,&capacity) == 3){
        adjacent_matrix[u][v] = capacity;
        current_edge_number++;
    }

    *edge_number = current_edge_number;

    return adjacent_matrix;

}

/**
 * copy an adjacent matrix
 */
int **copy_adjacent_matrix(int node_number,int** adjacent_matrix){
    int i,j;
    int **new_adjacent_matrix;

    //allocate the adjacent matrix (use malloc to save time)
    new_adjacent_matrix = (int **)(malloc(sizeof(int*) * (node_number)));
    for(i=0;i<node_number;i++){
        new_adjacent_matrix[i] = (int *)(malloc(node_number *sizeof(int)));
    }

    for(i=0;i<node_number;i++){
        for(j=0;j<node_number;j++){
            new_adjacent_matrix[i][j] = adjacent_matrix[i][j];
        }
    }

    return new_adjacent_matrix;
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

    //create new queue node
    Qnode = new_node(node);
    
    if(queue->head == NULL){
        //if it is the first node
        queue->head = Qnode;
        queue->last = Qnode;
    }else{
        //add it to the tail of queue
        queue->last->next = Qnode;
        queue->last = Qnode;
    }
    (queue->length)++;

}

// pop the first element and return node
int pop(Queue *queue){

    int node;

    if(queue->head==NULL){
        printf("ERROR: try to pop from an empty queue!");
        return -1;
    }

    Queue_Node *temp = queue->head;
    node = temp->node;
    queue->head = queue->head->next;
    free(temp);
    (queue->length)--;

    //queue is empty
    if(queue->head == NULL){
        queue->last = NULL;
    }
    
    return node;
}

//init the sequential data
Sequential_Data *sequential_init(int node_number, int **adjacent_matrix){
    int i,j;

    // create data
    Sequential_Data *temp = (Sequential_Data *)malloc(sizeof(Sequential_Data));
    temp->adjacent_matrix = adjacent_matrix;
    temp->node_number = node_number;
    // malloc space
    temp->d = (int *)malloc(node_number * sizeof(int)); 
    temp->excess = (int *)malloc(node_number * sizeof(int));
    temp->flows = create_adjacent_matrix(node_number);
    temp->active_nodes = create_queue();

    return temp;

}


//init the flow for reuse the data
void sequential_init_flow(Sequential_Data *data, int s, int t){
    int i,j;

    // updata s,t
    data->s = s;
    data->t = t;
    //clean the data
    for (i=0;i<data->node_number;i++) { 
        data->d[i] = 0; 
        data->excess[i] = 0;
        for (j=0;j<data->node_number;j++) { 
            data->flows[i][j] = 0;
        }
    }

    // saturating all the edges sv coming out the source s
    data->d[data->s] = data->node_number;
    for(i=0;i<data->node_number;i++){
        if(i == s){
            continue;
        }
        if(data->adjacent_matrix[s][i] != 0){
            // saturating the capacity
            data->flows[s][i] = data->adjacent_matrix[s][i];
            // excess increase by capacity of sv
            data->excess[i] = data->adjacent_matrix[s][i];
            // add residual flow
            data->flows[i][s] = -(data->flows[s][i]);
            // node will become active
            if(i != data->t){
                push(data->active_nodes,i);
            }
        }
    }

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
            if(v != data->s){
                //check whether the node v will become active
                if(data->excess[v] == 0 && v != data->t){
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
int sequential_pushrelabel(Sequential_Data *data, int s, int t){
    int u;

    //init flow
    sequential_init_flow(data,s,t);

    //do the while loop
    while(!isEmpty(data->active_nodes)){
        u = pop(data->active_nodes);

        while(data->excess[u] > 0){
            if(!sequential_push(data,u)){
                sequential_relabel(data,u);
            }
        }
    }

    //printf("%d to %d : %d\n", s, t, data->excess[t]);
    return data->excess[t];
}


// sequential version of all pair max flow
void sequential_all_pair(int node_number, int **adjacent_matrix){
    int i,j,minimum = INT_MAX;
    
    //init
    Sequential_Data *data = sequential_init(node_number,adjacent_matrix);

    double start_time = omp_get_wtime();

    for(i=0;i<node_number;i++){
        for(j=0;j<node_number;j++){
            if(i != j){
                minimum = min(minimum,sequential_pushrelabel(data,i,j));
            }
        }
    }

    double end_time = omp_get_wtime();

    //print the minimal
    printf("%d ",minimum);

    //print time
    printf("%f ",end_time-start_time);

    //print new line
    printf("\n",minimum);

    //free the data
    free_sequential_data(data);
}

// parallel version of all pair max flow
void parallel_all_pair(int node_number, int **adjacent_matrix, int number_thread){
    int i,j,minimum = INT_MAX;
    
    double start_time = omp_get_wtime();

    #pragma omp parallel for collapse(2) num_threads(number_thread) reduction(min:minimum)
    for(i=0;i<node_number;i++){
        for(j=0;j<node_number;j++){
            if(i != j){
                //init,  every thread need to have its own data
                Sequential_Data *data = sequential_init(node_number,adjacent_matrix);
                minimum = min(minimum,sequential_pushrelabel(data,i,j));
                //free the data
                free_sequential_data(data);
            }
        }
    }

    double end_time = omp_get_wtime();

    //print the minimal
    printf("%d ",minimum);

    //print time
    printf("%f ",end_time-start_time);

    //print new line
    printf("\n");
    
}

// // parallel version of all pair max flow with some optimization
// void parallel_op_all_pair(int node_number, int **adjacent_matrix, int number_thread){
//     int i,j,minimum = INT_MAX;
    
//     double start_time = omp_get_wtime();

//     Sequential_Data *data = sequential_init(node_number,adjacent_matrix);

//     #pragma omp parallel for dynamic nonmonotonic collapse(2) num_threads(number_thread) reduction(min:minimum) threadprivate(data)
//     for(i=0;i<node_number;i++){
//         for(j=0;j<node_number;j++){
//             if(i != j){
                
//                 minimum = min(minimum,sequential_pushrelabel(data,i,j));
//                 //free the data
//                 free_sequential_data(data);
//             }
//         }
//     }

//     double end_time = omp_get_wtime();

//     //print the minimal
//     printf("%d ",minimum);

//     //print time
//     printf("%f ",end_time-start_time);

//     //print new line
//     printf("\n");
    
// }


