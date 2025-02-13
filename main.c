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
#include <stdio.h>
#include <time.h>

#include "libgpiod.h"

int arr[10] = {0,1,1,1,0,0,0,1,1,0};

// Structure to hold thread data
struct thread_data
{
    int thread_num;
    gpiod_line_s* line;
};

void schlafe(int sec, int nsec)
{
    // 1000 nano sec = 1 micro Sec
    struct timespec ts;
    ts.tv_sec = sec; // Sekunden
    ts.tv_nsec = nsec; // Nanosekunden (10 Millisekunden = 10.000.000 Nanosekunden)
    nanosleep(&ts, NULL);
}

// Thread function
void* thread_function(void* arg) {
    struct thread_data thread_data = *((struct thread_data*)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    int anzahl = 0;
    for(;;)
    {
        if(anzahl % 2 == 0)
        {
            // Set GPIO line to high
            schlafe(0,300000);
            set_gpio(thread_data.line, 1);

            schlafe(0,200000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            schlafe(0,300000);
        } else
        {
            // Set GPIO line to high
            schlafe(0,200000);
            set_gpio(thread_data.line, 1);

            schlafe(0,400000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            schlafe(0,200000);
        }

        anzahl++;

    }
}

// Thread function
void* thread_function2(void* arg) {
    struct thread_data thread_data = *((struct thread_data*)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    int anzahl = 0;

    for(;;)
    {
        if(arr[0] == 0)
        {
            // Set GPIO line to high
            schlafe(0,300000);
            set_gpio(thread_data.line, 1);

            schlafe(0,200000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            schlafe(0,300000);
        } else
        {
            // Set GPIO line to high
            schlafe(0,200000);
            set_gpio(thread_data.line, 1);

            schlafe(0,400000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            schlafe(0,200000);
        }
        if (anzahl < 10)
        {
            anzahl++;
        }
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

    if (pthread_create(&thread2, NULL, thread_function2, (void*)&thread2_args) != 0) {
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
