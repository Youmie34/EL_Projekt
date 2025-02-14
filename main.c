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
#include "IPC/helper.h"

#include "libgpiod.h"

int senden = 0;

int arr[10] = {1, 0, 0, 1, 1, 1, 0, 1, 0, 1}; // test array
int message[40];                              // array mit mindestens 40 Einträgen

/*
void stringToBinaer(uint8_t *array, const char *binaryString)
{
    size_t length = strlen(binaryString);
    for (size_t i = 0; i < length; i++)
    {
        if (binaryString[i] == '0')
        {
            array[i] = 0;
        }
        else if (binaryString[i] == '1')
        {
            array[i] = 1;
        }
        else
        {
            printf("Ungültiges Zeichen: %c\n", binaryString[i]);
            return;
        }
    }
}
*/

// Structure to hold thread data
struct thread_data
{
    int thread_num;
    gpiod_line_s *line;
};

// Thread function
void *thread_function(void *arg)
{
    struct thread_data thread_data = *((struct thread_data *)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    int anzahl = 0;
    for (;;)
    {
        /*if(anzahl % 2 == 0)
        {
            // Set GPIO line to high
            sleep_custom(0,300000);
            set_gpio(thread_data.line, 1);

            sleep_custom(0,200000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            sleep_custom(0,300000);
        } else
        {
            // Set GPIO line to high
            sleep_custom(0,200000);
            set_gpio(thread_data.line, 1);

            sleep_custom(0,400000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            sleep_custom(0,200000);
        }*/

        if (senden == 0)
        {
            set_gpio(thread_data.line, 0);
        }
        else
        {
            set_gpio(thread_data.line, 1);
        }

        anzahl++;
    }
}

// Thread function
void *thread_function2(void *arg)
{
    struct thread_data thread_data = *((struct thread_data *)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    int anzahl = 0;

    senden = 1;
    for (;;)
    {
        if (message[anzahl] == 0)
        {
            // Set GPIO line to high
            sleep_custom(0, 300000);
            set_gpio(thread_data.line, 1);

            sleep_custom(0, 200000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            sleep_custom(0, 300000);
        }
        else
        {
            // Set GPIO line to high
            sleep_custom(0, 200000);
            set_gpio(thread_data.line, 1);

            sleep_custom(0, 400000);

            // Set GPIO line to low
            set_gpio(thread_data.line, 0);
            sleep_custom(0, 200000);
        }
        if (anzahl < 32)
        {
            anzahl++;
        }
        else
        {
            senden = 0;
        }
    }
}

int main()
{

    // Message inistialisieren
    // SendeID 0X99 1001 1001 8 Bit
    message[0] = 1;
    message[1] = 0;
    message[2] = 0;
    message[3] = 1;
    message[4] = 1;
    message[5] = 0;
    message[6] = 0;
    message[7] = 1;

    // Wert-ID1 4 Bit
    message[8] = 0;
    message[9] = 0;
    message[10] = 0;
    message[11] = 1;

    // Wert1: 8 Bit

    /*int sensorValue1 = readSensorValue(SHM_Sensor_1);
    if (sensorValue1 != -1)
    {
        decimalToBinary(message, 12, sensorValue1);
    }
    */

    // TODO: implement function that calls randomNumerGenerator, turns that number from
    // dec in binary and saves it in the next 8 bit in the message array

    // Wert-ID 1 4 Bit
    message[20] = 0;
    message[21] = 0;
    message[22] = 1;
    message[23] = 0;

    // TODO: convert decimal numbers from shm_ptr in binary and add it to message[12]

    /*int sensorValue2 = readSensorValue(SHM_Sensor_2);
    if (sensorValue2 != -1)
    {
        decimalToBinary(message, 24, sensorValue2);
    }
    */
    // TODO: add CRC

    pthread_t thread1, thread2;
    struct thread_data thread1_args, thread2_args;

    // Initialize first thread
    thread1_args.thread_num = 1;
    thread1_args.line = init_gpio("gpiochip0", 78);

    if (pthread_create(&thread1, NULL, thread_function, (void *)&thread1_args) != 0)
    {
        perror("Failed to create thread 1");
        return 1;
    }

    if (pthread_setname_np(thread1, "myThread_1") != 0)
    {
        perror("Cannot set name for thread 1");
    }

    // Initialize second thread
    thread2_args.thread_num = 2;
    thread2_args.line = init_gpio("gpiochip0", 81);

    if (pthread_create(&thread2, NULL, thread_function2, (void *)&thread2_args) != 0)
    {
        perror("Failed to create thread 2");
        return 1;
    }

    if (pthread_setname_np(thread2, "myThread_2") != 0)
    {
        perror("Cannot set name for thread 2");
    }

    // Wait for threads to finish
    if (pthread_join(thread1, NULL) != 0)
    {
        perror("Failed to join thread 1");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0)
    {
        perror("Failed to join thread 2");
        return 1;
    }

    printf("All threads have finished.\n");
    return 0;
}

void decimalToBinary(uint8_t *array, int startIndex, int decimalNumber)
{
    for (int i = 0; i < 8; i++)
    {
        array[startIndex + i] = (decimalNumber >> (7 - i)) & 1;
    }
}