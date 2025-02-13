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
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "libgpiod.h"

int senden = 0;

int arr[10] = {1,1,0,0,0,1,0,1,0,1};
int message[32];

void stringToBinaer(uint8_t *array, const char *binaryString) {
    size_t length = strlen(binaryString);
    for (size_t i = 0; i < length; i++) {
        if (binaryString[i] == '0') {
            array[i] = 0;
        } else if (binaryString[i] == '1') {
            array[i] = 1;
        } else {
            printf("Ungültiges Zeichen: %c\n", binaryString[i]);
            return;
        }
    }
}

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
        /*if(anzahl % 2 == 0)
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
        }*/

        if(senden == 0)
        {
            set_gpio(thread_data.line, 0);
        } else
        {
            set_gpio(thread_data.line, 1);
        }

        anzahl++;

    }
}

// Thread function
void* thread_function2(void* arg) {
    struct thread_data thread_data = *((struct thread_data*)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    int anzahl = 0;

    senden = 1;
    for(;;)
    {
        if(message[anzahl] == 0)
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
        if (anzahl < 32)
        {
            anzahl++;
        } else
        {
            senden = 0;
        }
    }
}

int main() {

    // Message inistialisieren
    // ID 0X99 1100 011 8 Bit
    message[0] = 0;
    message[1] = 1;
    message[2] = 1;
    message[3] = 0;
    message[4] = 0;
    message[5] = 0;
    message[6] = 1;
    message[7] = 1;

    // Wert-ID 1 4 Bit
    message[8] = 0;
    message[9] = 0;
    message[10] = 0;
    message[11] = 1;

    // Wert 1 8 Bit

    // Wert-ID 1 4 Bit
    message[20] = 0;
    message[21] = 0;
    message[22] = 1;
    message[23] = 0;
    // Wert 8 Bit

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
