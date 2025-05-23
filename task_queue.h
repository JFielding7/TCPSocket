#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

typedef struct task_queue_s* task_queue_t;

task_queue_t task_queue_create();

void task_queue_free(task_queue_t queue);

#endif //TASK_QUEUE_H
