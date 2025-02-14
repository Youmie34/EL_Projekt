#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // Für O_* Konstanten
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h> // Für O_CREAT
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h> // Für wait()
#include "prozess_2.h"
#include "helper.h"

#define SHM_SIZE 1024
// #define SHM_Sensor_1 "/shared_memory_Sensor_1"
#define SHM_Sensor_2 "/shared_memory_Sensor_2"
#define SEM_Prozess_2 "/SEM_Prozess_2"

void prozess2()
{
    std::cout << "Prozess 2 Start" << std::endl;

    sem_t *sem2 = sem_open(SEM_Prozess_2, 0); // Öffnet das bestehende Semaphore
    if (sem2 == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    std::cout << "Prozess 2" << std::endl;

    // 1. Shared Memory erstellen
    int shm_fd = shm_open(SHM_Sensor_2, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // 2. Größe des Shared Memory festlegen
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    // 3. Shared Memory in den Adressraum mappen als int
    void *shm_ptr2 = (int *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr2 == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // sem_wait(sem2);

    // 4. Daten schreiben

    srand(10);

    // for (int i = 0; i < 10; i++)
    for (;;)
    {
        int random_number = rand() % 256; // Zufallszahl zwischen 0 und 255

        // CHANGED
        *shm_ptr2 = random_number; // Direkt als Integer schreiben
        // snprintf((char *)shm_ptr2, SHM_SIZE, "%d", random_number);
        printf("Prozess 2 gespeichert: %d\n", random_number);
        struct timespec ts = {0, 100000000}; // 1 Millisekunde
        nanosleep(&ts, NULL);
    }
    // sem_post(sem2);

    // 5. Shared Memory entmappen und schließen
    munmap(shm_ptr2, SHM_SIZE);
    close(shm_fd);
}