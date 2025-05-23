#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "thread_pool.h"

#include "task_queue.h"

struct thread_pool_s {
    int thread_count;
    pthread_t *threads;
    pthread_cond_t cond;
    task_queue_t task_queue;
};


void* worker_thread(void *args) {
    puts("Hello");
    return NULL;
}


thread_pool_t thread_pool_create(int thread_count) {
    thread_pool_t thread_pool = malloc(sizeof(struct thread_pool_s));
    thread_pool->thread_count = thread_count;

    thread_pool->threads = malloc(thread_count * sizeof(pthread_t));
    for (int i = 0; i < thread_count; i++) {
        pthread_create(thread_pool->threads + i, NULL, &worker_thread, NULL);
    }

    pthread_cond_init(&thread_pool->cond, NULL);

    thread_pool->task_queue = task_queue_create();

    return thread_pool;
}


void thread_pool_destroy(thread_pool_t thread_pool) {
    for (int i = 0; i < thread_pool->thread_count; i++) {
        pthread_join(thread_pool->threads[i], NULL);
    }

    pthread_cond_destroy(&thread_pool->cond);
    task_queue_free(thread_pool->task_queue);
    free(thread_pool->threads);
    free(thread_pool);
}
