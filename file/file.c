// file.c
#include "file.h"
#include <stdlib.h>
#include <stdio.h>

// Crée une nouvelle file
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Ajoute un élément à la file
void enqueue(Queue* queue, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Retire un élément de la file
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Erreur : la file est vide\n");
        exit(EXIT_FAILURE);
    }

    Node* temp = queue->front;
    int data = temp->data;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return data;
}

// Vérifie si la file est vide
int isEmpty(Queue* queue) {
    return queue->front == NULL ? 1:0;
}

// Libère la mémoire allouée pour la file
void freeQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

// Affiche tous les éléments de la file pour le débogage
void printQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("La file est vide.\n");
        return;
    }

    printf("Contenu de la file : ");
    Node* current = queue->front;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}