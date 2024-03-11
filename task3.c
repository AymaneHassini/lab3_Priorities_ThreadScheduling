#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>

#define NUM_THREADS 5

// Task types
typedef enum
{
    TASK_EMERGENCY,
    TASK_USER_STANDARD,
    TASK_BACKGROUND
} TaskType;

// Task structure
typedef struct
{
    TaskType type;
    int priority;
    int id;
} Task;

// Function prototypes
Task *createTask(TaskType type, int priority, int id);
void deleteTask(Task *task);
void *executeTask(void *arg);
const char *getTaskTypeName(TaskType type);

// Task creation with memory management
Task *createTask(TaskType type, int priority, int id)
{
    Task *task = (Task *)malloc(sizeof(Task));
    if (task == NULL)
    {
        fprintf(stderr, "Error allocating memory for task!\n");
        return NULL;
    }
    task->type = type;
    task->priority = priority;
    task->id = id;
    return task;
}

// Cleanup after task execution
void deleteTask(Task *task)
{
    if (task != NULL)
    {
        free(task);
    }
}

// Task execution function with error handling
void *executeTask(void *arg)
{
    Task *task = (Task *)arg;
    printf("Task %d (Priority: %d) of type %s\n", task->id, task->priority, getTaskTypeName(task->type));
    sched_yield();

    // Simulate task execution
    usleep(1000);

    deleteTask(task);
    pthread_exit(NULL);
}

// Helper to get human-readable task type names
const char *getTaskTypeName(TaskType type)
{
    switch (type)
    {
    case TASK_EMERGENCY:
        return "Emergency Service";
    case TASK_USER_STANDARD:
        return "Standard User Request";
    case TASK_BACKGROUND:
        return "Background Maintenance";
    default:
        return "Unknown";
    }
}

int main()
{
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    struct sched_param param;
    int ret;

    // Initialize thread attribute
    pthread_attr_init(&attr);

    // Enable explicit scheduling settings in attribute
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    // Set scheduling policy to SCHED_FIFO
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    // Create tasks with varying priorities
    Task *tasks[NUM_THREADS];
    tasks[0] = createTask(TASK_EMERGENCY, 10000, 2);
    tasks[1] = createTask(TASK_USER_STANDARD, 1000, 3);
    tasks[2] = createTask(TASK_EMERGENCY, 8000, 1);
    tasks[3] = createTask(TASK_BACKGROUND, 10, 5);
    tasks[4] = createTask(TASK_USER_STANDARD, 2000, 4);

    // Error handling during task creation
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (tasks[i] == NULL)
        {
            fprintf(stderr, "Error creating task %d\n", i);
            // Consider more specific error handling here
        }
    }

    // Create threads, setting priorities, and passing tasks
    for (int i = 0; i < NUM_THREADS; i++)
    {
        param.sched_priority = tasks[i]->priority;
        pthread_attr_setschedparam(&attr, &param);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        ret = pthread_create(&threads[i], &attr, executeTask, tasks[i]);
        if (ret)
        {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    pthread_attr_destroy(&attr);

    return 0;
}