/*-------------------------------------------
This C code is written for assignment 4
Programming, Bridging Course, 10.0 c
HT-2020 / nara5324@student.uu.se (Naeim)
-------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifndef __queue__
#define __queue__
//https://codereview.stackexchange.com/questions/141238/implementing-a-generic-queue-in-c
//Declation of data types and functions
    typedef void* Data; //an unspecified pointer, to be able to store any data type
    typedef struct queueElem{
        Data item;
        int data_type; // used to print elements with correct format specifiers 2(float) 3(string)
        struct queueElem* next; //pointer for the next node
    } queueElem, *link; //link is pointer to each queueElem

    /* Data is a simple void data type such as: char, int, float, double or a pointer to arrays
    queueElem combines Data with a pointer to make it possible for linked list creation
    But QueueRecord helps to link different linked-lists inside one run! */
    typedef struct QueueRecord{ //consists of Three pointers 24bite
        struct queueElem* head; // pointer to head of each Queue
        struct queueElem *tail;// link tail; //pointer to tail of each Queue
        int queue_counter; //to keep truck of number of queues in the program but not size of one queue
        link curser;
        struct QueueRecord* next; // To keep track of different Queues lined to eachother
    } QueueRecord, *Queue;

    Queue newQueue(); // A linked-list of QueueRecord struct type
    void enqueue(Queue q,link elem); //Put an element in the queue.
    Data dequeue(Queue q); //Removes and returns the first element from the queue, Returns NULL if the queue is empty.
    Data first(Queue q); //Returns the first element but leaves it in the queue, Returns NULL if the queue is empty.
    int qLength(Queue q); //Returns the number of elements in the queue.
    void qitReset(Queue q); //Positions the iterator to the first element in the queue.
    int qitMore(Queue q); //Returns 0 if all elements have been returned by the iterator, else 1
    Data qitNext(Queue q); //Returns the current element and advances to the next.
    link user_iteraction(); //To read one word from standard input and return a queueElem
    void instructions(); // help content
    void queue_iterator(Queue q);

#endif

Queue main_head = NULL; // acts as head for linked-list of several queues
#define NEW_QUEUE "NQ"
#define TERMINATION "QUIT"
#define FIRST "FIRST"
#define PRINT "PRINT"

int main(){

    instructions();
    printf("Enter one word and press Enter!\n");
    Queue q = newQueue(); //creating the default queue to work with
    link element = NULL; //pointer to save the address of user input

    while(1){
        element = user_iteraction();
        if(!strcmp((char*)(element->item),NEW_QUEUE)){
            q = newQueue();
            free(element);//To prevent saving 'NQ' into Queue
        }
        else if(!strcmp((char*)(element->item),FIRST)){
            if(first(q) == NULL){
                continue;
            }
            else if(q->head->data_type == 2)
                printf("First element in #%d Q is:%.3f\n",q->queue_counter,*(float*)first(q));
            else
                printf("First element in #%d Q is:%s\n",q->queue_counter,(char*)first(q));

            free(element);//To prevent saving 'FIRST' into Queue
        }
        else if(!strcmp((char*)(element->item),PRINT)){
            queue_iterator(q);
            free(element);//To prevent saving 'PRINT' into Queue
        }
        else if(!strcmp((char*)(element->item),TERMINATION)){
            int print_empty_queue; //default is TRUE
            printf("\nDEQUEUE\n");
            while(main_head != NULL){
                print_empty_queue = 1;
                printf("------\n");
                while(qLength(main_head)>0){
                    if(main_head->head->data_type == 2)
                        printf("Q%d:%.3f\n",main_head->queue_counter,*(float*)dequeue(main_head));
                    else
                        printf("Q%d:%s\n",main_head->queue_counter,(char*)dequeue(main_head));
                    print_empty_queue = 0;
                }
                if(qLength(main_head) == 0 && print_empty_queue){
                    printf("Q%d is EMPTY!\n",main_head->queue_counter);
                    print_empty_queue = 1; //set the check to TRUE
                }
                main_head = main_head->next;
            }
            free(element);//To prevent saving 'QUIT' into Queue
            return 0;
        }
        else{
            enqueue(q,element);
            printf("Q%d:(#%d element)\n",q->queue_counter,qLength(q));
        }
    }

    return 0;
}

Queue newQueue(){//create a linked list of queues
    static int queue_counter = 1;

    if(main_head == NULL){
        Queue new_Queue = (Queue)malloc(sizeof(QueueRecord));
        if(new_Queue == NULL){// in case no memory left in computer!
            printf("Error creating a new queue.\n");
            exit(0);
        }
        new_Queue->head = new_Queue->tail = NULL;// initializing the new queue
        main_head = new_Queue; // storing address of first queue in main_head pointer
        main_head->next = NULL; // point to NULL since no other queue exists
        main_head->queue_counter = queue_counter; // first node in linked-list of queues
        printf("#%d queue created!\n",queue_counter);
        return new_Queue;
    }

    else{ //if there is already another queue
        Queue last_queue = main_head; // in oreder to find the last queue in linked list of queues
        queue_counter++; //int value in each queue to be found by user input

        while(last_queue->next != NULL){
            last_queue = last_queue->next;
        }

        Queue new_Queue = (Queue)malloc(sizeof(QueueRecord));
        new_Queue->head = new_Queue->tail = NULL;// initializing the new queue
        new_Queue->next = NULL; // point to NULL since no other queue exists afterwards
        last_queue->next = new_Queue; // linking new_Queue to previous one

        new_Queue->queue_counter = queue_counter;
        printf("----------\n#%d queue created!\n",queue_counter);
        return new_Queue;
    }
}

void enqueue(Queue queue, queueElem* elem){
        if(queue->head == NULL){ // current queue is empty
            queue->head = elem;
            queue->tail = elem;
        }
        else{
            queue->tail->next = elem;
            queue->tail = elem;

        }
}

Data dequeue(Queue q){
    link wipe_data = q->head;
    if(q->head == NULL){
        printf("The queue is empty!\n");
        return NULL;
    }
    else{
        Data temp_item = q->head->item; // creating temp_item to save item in head node for returning
        q->head = q->head->next; //incrementing the head node to the next node in the queue
        free(wipe_data);//freeing old node-space to not wasting any memory behind
        return temp_item; // returning the item
    }
}

Data first(Queue q){
    if(q->head == NULL){
        printf("EMPTY QUEUE!\n");
        return NULL;
    }
    else
        return q->head->item;
}

int qLength(Queue q){
    if(q->head == NULL)
        return 0;// the queue is empty
    else{
        int queue_size = 1;
        link inner_curser = q->head;//to not change the right address of head node!
        while(inner_curser != q->tail){
            queue_size++;
            inner_curser = inner_curser->next;
            //printf("queue_size is:%d\n",queue_size);
        }
        return queue_size;
    }
}

void qitReset(Queue q){
    q->curser = q->head;
}

//Returns 0 if all elements have been returned by the iterator, else 1
int qitMore(Queue q){

    if(q->curser == NULL)
        return 0;
    else
        return 1;
};

Data qitNext(Queue q){
    Data temp = q->curser->item;
    q->curser = q->curser->next;
    return temp;
}

link user_iteraction(){
    //Function to read one word at a time
    char input[30] = {0};
    int check_for_number = 0; //to distingish numbers from strings
    int one_dot = 0; // to allow only one '.' (1 means true!)
    static int input_counter = 1;
    //Read a word from stdin

    printf("INPUT #%d:",input_counter);
    scanf("%s",input); //stops reading as soon as it encounters a space

    for(int i=0;i<30;i++){//returns non-zero value if input[i] is a digit, else it returns 0
        if(isdigit(input[i]) || input[i]=='.')
            check_for_number = 1;

        if(input[i]=='.')
            one_dot++;

        if(isalpha(input[i]) || ispunct(input[i])){
            if(one_dot == 1 || i ==0)// to prevent two .. or to keep - in the beginning
                continue;
            else{
                check_for_number = 0;
                break;
            }
        }
    }

    //printf("---check_for_number is:%d\n",check_for_number);
    if(check_for_number){

        float* number = (float*)malloc(sizeof(float));
        *number = atof(input);//converts string to float

        link data_float = (link)malloc(sizeof(queueElem));
        data_float->item = number; //a float pointer into a void type pointer
        (*data_float).next = NULL;
        data_float->data_type = 2;

        //printf("#%d queueElem(float): %.4f\n",input_counter,*((float*)(data_float->item)));
        input_counter++;
        return data_float;
    }
    else{
        char* string = (char*)malloc(sizeof(char));
        strcpy(string,input);

        link data_string = (link)malloc(sizeof(queueElem));
        data_string->item = string;
        data_string->next = NULL;
        data_string->data_type = 3;

        //printf("#%d queueElem(string): %s\n",input_counter,(char*)(data_string->item));
        input_counter++;
        return data_string;
    }
}

void queue_iterator(Queue qx){
    if(qx->head == NULL){
        printf("EMPTY QUEUE!\n");
        return;
    }

    qitReset(qx);
    while(qitMore(qx)){
        if(qx->curser->data_type == 3)
            printf("\"%s\" ", (char *)qitNext(qx));
        else
            printf("\"%.3f\" ", *(float *)qitNext(qx));
    if(qitMore(qx)){
        printf(", ");
        }
    }
    printf("\n");
}

void instructions(){

    printf("----------------------\nINSTRUCTIONS\n");
    printf("The program creates one queue by default.\n");
    printf("It is possible to save any data type in all queues.\n");
    printf("The program can differentiate between a 'number' and a 'string'.\n");
    printf("Extra queues are possible by typing %s. Then, entries are saved in the new queue.\n",NEW_QUEUE);
    printf("Type %s to see the first element in current queue.\n",FIRST);
    printf("It is up to the user to keep data types seperated if intended.\n");
    printf("To iterate over elements of current queue enter %s.\n",PRINT);
    printf("Enter %s to dequeue existing queue(s) and terminate the program.\n",TERMINATION);
    printf("----------------------\n");

};