#include "../header/gpio_control.h"
#include "../header/helper.h"

int senden = 0;



// Thread function
// TODO: implement code for clock
void *thread_function(void *arg)
{
    struct thread_data thread_data = *((struct thread_data *)arg);
    printf("Hello from thread %d\n", thread_data.thread_num);

    int anzahl = 0;
    for (;;)
    {
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

void control_gpios(int *message, int size)
{
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