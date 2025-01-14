#ifndef FILE_H
#define FILE_H

// Structure pour un élément de la file
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structure pour la file
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Prototypes des fonctions
Queue* createQueue();
void enqueue(Queue* queue, int value);
int dequeue(Queue* queue);
int isEmpty(Queue* queue);
void freeQueue(Queue* queue);
void printQueue(Queue* queue);

#endif // FILE_H
