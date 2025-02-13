/**
 * @file main.c
 * @brief This program creates multiple threads to control GPIO lines using the libgpiod library.
 *
 * Each thread toggles a specific GPIO line on and off in an infinite loop.
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "libgpiod.h"

// Structure to hold thread data
struct thread_data
{
    int thread_num;
    gpiod_line_s* line;
};

// Thread function
void* thread_function(void* arg) {
    struct thread_data thread_data = *((struct thread_data*)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    for(;;)
    {
        // Set GPIO line to high
        set_gpio(thread_data.line, 1);
        // Set GPIO line to low
        set_gpio(thread_data.line, 0);
    }
}

int main() {
    pthread_t thread1, thread2;
    struct thread_data thread1_args, thread2_args;

    // Initialize first thread
    thread1_args.thread_num = 1;
    thread1_args.line = init_gpio("gpiochip0", 78);

    if (pthread_create(&thread1, NULL, thread_function, (void*)&thread1_args) != 0) {
        perror("Failed to create thread 1");
        return 1;
    }

    if (pthread_setname_np(thread1, "myThread_1") != 0) {
        perror("Cannot set name for thread 1");
    }

    // Initialize second thread
    thread2_args.thread_num = 2;
    thread2_args.line = init_gpio("gpiochip0", 81);

    if (pthread_create(&thread2, NULL, thread_function, (void*)&thread2_args) != 0) {
        perror("Failed to create thread 2");
        return 1;
    }

    if (pthread_setname_np(thread2, "myThread_2") != 0) {
        perror("Cannot set name for thread 2");
    }

    // Wait for threads to finish
    if (pthread_join(thread1, NULL) != 0) {
        perror("Failed to join thread 1");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("Failed to join thread 2");
        return 1;
    }

    printf("All threads have finished.\n");
    return 0;
}
