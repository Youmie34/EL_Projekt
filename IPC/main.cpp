#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/shm.h>
#include <iostream>
#include <fcntl.h> // Für O_CREAT
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h> // Für wait()
#include "prozess_1.h"
#include "prozess_2.h"
#include "prozess_3.h"

#define SHM_SIZE 1024
#define SHM_Sensor_1 "/shared_memory_Sensor_1"
#define SHM_Sensor_2 "/shared_memory_Sensor_2"
#define SEM_Prozess_1 "/SEM_Prozess_1"
#define SEM_Prozess_2 "/SEM_Prozess_2"

int main()
{
    pid_t pid1 = fork();

    if (pid1 < 0)
    {
        perror("Fork failed");
        return 1; // Fehler beim Erstellen des Prozesses
    }
    if (pid1 == 0)
    {
        // Prozess 1
        prozess1();
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            perror("Fork failed");
            return 1; // Fehler beim Erstellen des Prozesses
        }
        if (pid2 == 0)
        {
            // Prozess 2
            prozess2();
        }
        else
        {
            // Prozess 3
            prozess3();

            //wait(NULL);           // Warten auf das Kind
        }
    }

    return 0;
}