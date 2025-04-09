#ifndef Q_H
#define Q_H

#include "AL.h" 

// typedef struct ArrayList {
//     int *array;
//     int size;
//     int capacity;
// } AL;


typedef struct Queue {
    AL *list;
    int front;
} queue;

// Queue function declarations
void create_queue(queue **p_q);
void destroy_queue(queue *q);
void enqueue(queue *q, int data);
void dequeue(queue *q);
int front(queue *q);  // Optional: return front element
int is_empty(queue *q); // Optional: check if empty

#endif
