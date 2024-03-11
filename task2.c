#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#define SIZE 50
void *threadFunction(void *arg)
{
    int threadId = *(int *)arg;
    char message[SIZE];
    sprintf(message, "Thread %d with ...", threadId);

    for (int i = 0; i < 5; i++)
    {
        printf("%s: %d\n", message, i);
        sleep(1); // Sleep to simulate work
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[4]; // Array to hold threads
    pthread_attr_t attr[4];
    struct sched_param params[4];
    int threadIds[4]; // Array for thread IDs

    printf("Starting program...\n");

    // Initialize thread attributes
    for (int i = 0; i < 4; i++)
    {
        pthread_attr_init(&attr[i]);
    }

    // Set priorities and scheduling policy for all threads
    for (int i = 0; i < 4; i++)
    {
        threadIds[i] = i + 1;                               // Assign IDs 1, 2, 3, 4
        pthread_attr_setschedpolicy(&attr[i], SCHED_OTHER); // Start with SCHED_OTHER

        // Experiment with priorities within your policy's range
        params[i].sched_priority = i + 1;
        pthread_attr_setschedparam(&attr[i], &params[i]);
    }

    // Create threads
    for (int i = 0; i < 4; i++)
    {
        int ret = pthread_create(&threads[i], &attr[i], threadFunction, &threadIds[i]);
        if (ret)
        {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    for (int i = 0; i < 4; i++)
    {
        pthread_attr_destroy(&attr[i]);
    }

    printf("Program finished.\n");

    return 0;
}
