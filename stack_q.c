#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int numOfClient;

struct type{
    char *stackPtr;
    int stackSize, top;
    char *queuePtr;
    int front, rear, queueSize, queueCount;
    char History[1000];
};
/* this functions are stack's functions */
char pop(int*, char*);
void push(char, int*, char**, int);
int isfull( int, int);
int isempty(int);

/* this functions are queue's functions */
int IsFull(int, int);
int IsEmpty(int*, int*, int);
char Dequeue(char*, int*, int*, int*, int);
void Enqueue(char, char**, int*, int*, int*, int);

/* this function for reading first input file */
void readFile1(FILE*, char str[15], struct type*, struct type**);

/* this function for reading second input file */
void readFile2(FILE*, char str[15], struct type*, struct type**);

/* this function for writing output file */
void writeFile(FILE*, struct type*, struct type**);


int main(int argc, char **argv) {
    FILE *fp = fopen(argv[1], "r");
    /* reading first line in the first input file */
    char str[15];
    char *token;
    fgets(str, sizeof(str), fp);
    token = strtok(str,"\n");

    numOfClient = atoi(token)-1;

    struct type Server;
    /* used dynamic memory allocation for all of client */
    struct type *Client = (struct type*) malloc(numOfClient*sizeof(struct type));

    readFile1(fp, str, &(Server), &(Client));
    fclose(fp);

    fp = fopen(argv[2], "r");
    readFile2(fp, str, &(Server), &(Client));
    fclose(fp);

    fp = fopen(argv[3], "w");
    writeFile(fp, &(Server), &(Client));
    fclose(fp);


    return 0;
}

void readFile1(FILE *fp, char str[15], struct type *Server, struct type **Client)
{
    int i=0;

    while(fgets(str, 15*sizeof(char), fp))
    {
        if(i == numOfClient)/* last line in the first input file */
        {
            /* creating stack and queue for Server */
            Server->queuePtr = (char*) malloc( str[0] * sizeof(char));
            Server->front = -1;
            Server->rear = -1;
            Server->queueSize = atoi(str);
            Server->stackPtr = (char*) malloc( str[2] * sizeof(char));
            Server->top = -1;
            Server->stackSize = atoi(str+2);
            Server->queueCount = 0;
            Server->History[0] = '\0';
        }
        else{
            /* creating stack and queue for ech of Client */
            (*(Client)+i)->queuePtr = (char*) malloc( str[0] * sizeof(char));
            (*(Client)+i)->front = -1;
            (*(Client)+i)->rear = -1;
            (*(Client)+i)->queueSize = atoi(str);
            (*(Client)+i)->stackPtr = (char*) malloc( str[2] * sizeof(char));
            (*(Client)+i)->top = -1;
            (*(Client)+i)->stackSize = atoi(str+2);
            (*(Client)+i)->queueCount = 0;
            (*(Client)+i)->History[0] = '\0';
        }
        i++;
    }
}

void readFile2(FILE *fp, char str[15], struct type *Server, struct type **Client)
{
    /* read the first row in the second input file and it show how many command in the second input file */
    fgets(str, 15* sizeof(char), fp);
    char *temp = strtok(str, "\n");
    int numOfCommands = atoi(temp);

    int i = 0;

    while(i<numOfCommands)
    {
        fgets(str, 15*sizeof(char), fp);
        int n = (int) str[2]-49;

        if(str[0] == 'A') /* meaning of A is addition to Client's queue */
        {
            if(!IsFull( (*(Client) + n)->queueCount, (*(Client) + n)->queueSize )) {
                Enqueue( str[4], &( (*(Client) + n)->queuePtr ), &( (*(Client) + n)->front ),
                         &( (*(Client) + n)->rear ), &( (*(Client) + n)->queueCount ),
                         (*(Client) + n)->queueSize);
            } else {
                strcat((*(Client) + n)->History, "1 ");
            }
        }
        else if(str[0] == 'I') /* meaning of the I is addition to Stack or Client's stack */
        {
            if(n == numOfClient)/* Clients are finished add Serve's stack */
            {
                if(!isfull(Server->stackSize, Server->top)){
                    push(str[4], &(Server->top), &(Server->stackPtr), Server->stackSize);
                } else {
                    strcat(Server->History, "2 ");
                }
            } else{ /* add Client's stack */
                if(!isfull((*(Client) + n)->stackSize, (*(Client) + n)->top)){
                    push(str[4], &((*(Client) + n)->top), &((*(Client) + n)->stackPtr), (*(Client) + n)->stackSize);
                } else {
                    strcat((*(Client) + n)->History, "2 ");
                }
            }
        }
        else if(str[0] == 'S') /* S is used for adding to the Server's qeueu */
        {
            char *data = malloc(3* sizeof(char));
            char d;
            if(!isempty((*(Client) + n)->top))
            {
                d = pop(&((*(Client) + n)->top), (*(Client) + n)->stackPtr);
                sprintf(data, "%c ", d);
                strcat((*(Client) + n)->History, data);
                if(!IsFull(Server->queueCount, Server->queueSize)) {
                    Enqueue(d, &(Server->queuePtr), &(Server->front), &(Server->rear),
                            &(Server->queueCount), Server->queueSize);
                } else {
                    strcat(Server->History, "1 ");
                }
            }
            else
            {
                if(!IsEmpty(&((*(Client) + n)->front), &((*(Client) + n)->rear), (*(Client) + n)->queueCount)){
                    d = Dequeue((*(Client) + n)->queuePtr, &((*(Client) + n)->front),
                                &(*(Client) + n)->rear, &((*(Client) + n)->queueCount), (*(Client) + n)->queueSize);
                    sprintf(data, "%c ", d);
                    strcat((*(Client) + n)->History, data);
                    if(!IsFull(Server->queueCount, Server->queueSize)) {
                        Enqueue(d, &(Server->queuePtr), &(Server->front), &(Server->rear),
                                &(Server->queueCount), Server->queueSize);
                    } else {
                        strcat(Server->History, "1 ");
                    }
                } else {
                    strcat((*(Client) + n)->History, "3 ");
                }
            }
        }
        else if(str[0] == 'O') /* It operates the stack and queue of the server. */
        {
            char *data = malloc(3* sizeof(char));
            char d;
            if(!isempty(Server->top))
            {
                d = pop(&(Server->top), Server->stackPtr);
                sprintf(data, "%c ", d);
                strcat(Server->History, data);
            }
            else
            {
                if(!IsEmpty(&(Server->front), &(Server->rear), Server->queueCount)){
                    d = Dequeue(Server->queuePtr, &(Server->front), &(Server->rear),
                                &(Server->queueCount), Server->queueSize);
                    sprintf(data, "%c ", d);
                    strcat(Server->History, data);
                } else {
                    strcat(Server->History, "3 ");
                }
            }
        }
        i++;
    }
}

void writeFile(FILE *fp, struct type *Server, struct type **Client)
{
    int i = 0;

    while(i<numOfClient)/* this while loop is writing operations in the each of Client */
    {
        fprintf(fp, "%s\n", (*(Client)+i)->History);
        i++;
    }
    fprintf(fp, "%s", Server->History);/* the last line in the output file is operations in the Server */
}


int isfull( int stackSize, int top) { /* make the control for the stack is full */
    /* if the stack is full then return true. otherwise return false */
    if(top == (stackSize-1))
        return 1;
    else
        return 0;
}
int isempty(int top) { /* make the control for the stack is empty */
    /* if the stack is empty the return true. otherwise return false */
    if(top == -1)
        return 1;
    else
        return 0;
}
char pop(int *top,char *stack) { /* this function is used for delete stack's last element */
    char data;
    if(*(top) != -1) { /* if the stack is not empty tehn delete the element */
        data = *(stack + *(top));
        *(top) = *(top) - 1;
        return data;
    }
}
void push(char data, int* top, char** stack, int stackSize) { /* this function is used for add the element in the stack */
    if(!isfull(stackSize, *(top))) { /* if the stack is not full, the element add the stack */
        *(top) = *(top) + 1;
        *(*(stack) + *(top)) = data;
    }
}


int IsFull(int count, int queueSize) { /* this function control the queue. the queue is full return true. otherwise return false */
    if(count == queueSize){
        return 1;
    } else {
        return 0;
    }
}
int IsEmpty(int *front, int *rear, int count) { /* this function is used for the control of queue */
    /* if the queue is empty then return true. otherwise return false */
    if( ( *(rear) == -1 ) && ( *(front) == -1 ) ) {
        return 1;
    }
    else if( count == 0 ) {
        *(front) = -1;
        *(rear) = -1;
        return 1;
    }
    else {
        return 0;
    }
}
char Dequeue(char *qPtr, int *front, int *rear, int *count, int queueSize) {
    /* this function is for deleting in the queue's first element */
    char data;
    data = *(qPtr + *(front));
    *(front) = ( *(front) + 1 ) % queueSize;
    *(count) -= 1;
    return data;
}
void Enqueue(char data, char **qPtr, int *front, int *rear, int *count, int queueSize) {
    /* this function is used for the adding in the queue */
    if( ( *(front) == -1 ) && ( *(rear) == -1 ) ) { /* if the queue is empty */
        *(front) = 0;
        *(rear) = 0;
    }
    *( *(qPtr) + *(rear) ) = data;
    *(rear) = ( *(rear) + 1 ) % queueSize;
    *(count) += 1;
    return;
}