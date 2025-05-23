#ifndef THREAD_POOL_H
#define THREAD_POOL_H

typedef struct thread_pool_s* thread_pool_t;

thread_pool_t thread_pool_create(int thread_count);

#endif //THREAD_POOL_H
