#include <pthread.h>
#include "task_queue.h"

#include <stdlib.h>


typedef struct task_s {
    void* (*routine)(void *);
    void* args;
} task_t;


struct node_s {
    task_t task;
    struct node_s *next;
};
typedef struct node_s* node_t;


struct task_queue_s {
    node_t head;
    node_t tail;
    pthread_mutex_t mutex;
};


task_queue_t task_queue_create() {
    task_queue_t queue = calloc(1, sizeof(struct task_queue_s));
    pthread_mutex_init(&queue->mutex, NULL);

    return queue;
}

node_t node_create(task_t task) {
    node_t node = malloc(sizeof(struct node_s));
    node->task = task;
    node->next = NULL;

    return node;
}

void task_queue_push(task_queue_t queue, task_t task) {
    pthread_mutex_lock(&queue->mutex);

    node_t node = node_create(task);

    if (queue->head == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    pthread_mutex_unlock(&queue->mutex);
}

task_t task_queue_pop(task_queue_t queue) {
    pthread_mutex_lock(&queue->mutex);

    node_t head = queue->head;
    task_t task = head->task;
    queue->head = head->next;
    free(head);

    pthread_mutex_unlock(&queue->mutex);

    return task;
}

void task_queue_free(task_queue_t queue) {
    pthread_mutex_destroy(&queue->mutex);

    node_t node = queue->head;
    while (node != NULL) {
        node_t next = node->next;
        free(node);
        node = next;
    }

    free(queue);
}
