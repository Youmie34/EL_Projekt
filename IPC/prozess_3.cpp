#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // Für O_* Konstanten
#include <sys/mman.h>
#include <iostream>
#include <fcntl.h> // Für O_CREAT
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>
#include "prozess_3.h"

#define SHM_SIZE 1024
#define SHM_Sensor_1 "/shared_memory_Sensor_1"
#define SHM_Sensor_2 "/shared_memory_Sensor_2"
#define SEM_Prozess_1 "/SEM_Prozess_1"
#define SEM_Prozess_2 "/SEM_Prozess_2"

void prozess3()
{
    std::cout << "Prozess 3 Start" << std::endl;
    

    // 1. Shared Memory öffnen
    int shm_fd = shm_open(SHM_Sensor_1, O_RDONLY, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // 2. Shared Memory öffnen
    int shm_fd2 = shm_open(SHM_Sensor_2, O_RDONLY, 0666);
    if (shm_fd2 == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // 2. Shared Memory in den Adressraum mappen
    void *shm_ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // 2. Shared Memory in den Adressraum mappen
    void *shm_ptr2 = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd2, 0);
    if (shm_ptr2 == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    //sem_post(sem);

    // for(int i = 0; i < 10; i++)
    for(;;)
    {
        printf("Prozess 3\n");
        // 3. Daten lesen
        //sem_wait(sem);
        printf("Gelesene Daten 1: %s\n", (char *)shm_ptr);
        //sem_post(sem);

        // 3. Daten lesen
        //sem_wait(sem2);
        printf("Gelesene Daten 2: %s\n", (char *)shm_ptr2);
        //sem_post(sem2);
        printf("\n");
        struct timespec ts = {0, 100000000}; // 1 Millisekunde
        nanosleep(&ts, NULL);
    }

    // 4. Shared Memory entmappen und schließen
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    // Optional: Shared Memory löschen
    shm_unlink(SHM_Sensor_1);

        // 4. Shared Memory entmappen und schließen
    munmap(shm_ptr2, SHM_SIZE);
    close(shm_fd2);

    // Optional: Shared Memory löschen
    shm_unlink(SHM_Sensor_2);
}