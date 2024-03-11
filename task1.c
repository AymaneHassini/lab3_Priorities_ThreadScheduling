#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

void *threadFunction(void *arg)
{

    char *message = (char *)arg;

    for (int i = 0; i < 5; i++)
    {

        printf("%s: %d\n", message, i);

        sleep(1); // Sleep to simulate work
    }

    pthread_exit(NULL);
}

int main()
{

    pthread_t thread1, thread2;

    pthread_attr_t attr1, attr2;

    int ret;

    struct sched_param param1, param2;

    // Initialize thread attributes

    pthread_attr_init(&attr1);

    pthread_attr_init(&attr2);

    // Set scheduling policy to FIFO and priority

    pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);

    param1.sched_priority = sched_get_priority_max(SCHED_FIFO);

    pthread_attr_setschedparam(&attr1, &param1);

    // Set scheduling policy to RR and priority

    pthread_attr_setschedpolicy(&attr2, SCHED_RR);

    param2.sched_priority = sched_get_priority_min(SCHED_RR);

    pthread_attr_setschedparam(&attr2, &param2);

    // Create threads

    ret = pthread_create(&thread1, &attr1, threadFunction, "Thread 1 with high priority");

    if (ret)
    {

        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);

        exit(EXIT_FAILURE);
    }

    ret = pthread_create(&thread2, &attr2, threadFunction, "Thread 2 with low priority");

    if (ret)
    {

        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);

        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish

    pthread_join(thread1, NULL);

    pthread_join(thread2, NULL);

    // Clean up

    pthread_attr_destroy(&attr1);

    pthread_attr_destroy(&attr2);

    return 0;
}