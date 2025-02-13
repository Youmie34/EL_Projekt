#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // Für O_* Konstanten
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "prozess_1.h"
#include "helper.h"

#define SHM_SIZE 1024
#define SHM_Sensor_1 "/shared_memory_Sensor_1"
//#define SHM_Sensor_2 "/shared_memory_Sensor_2"
#define SEM_Prozess_1 "/SEM_Prozess_1"

char intInChar(int);

void schlafe(int sec, int nsec)
{
    // 1000 nano sec = 1 micro Sec
    struct timespec ts;
    ts.tv_sec = sec; // Sekunden
    ts.tv_nsec = nsec; // Nanosekunden (10 Millisekunden = 10.000.000 Nanosekunden)
    nanosleep(&ts, NULL);
}

void prozess1()
{
    std::cout << "Prozess 1 Start" << std::endl;

    // 1. Shared Memory erstellen
    int shm_fd = shm_open(SHM_Sensor_1, O_CREAT | O_RDWR, 0666);
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

    // 3. Shared Memory in den Adressraum mappen
    void *shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // 4. Daten schreiben

    srand(time(NULL));

    //for (int i = 0; i < 10; i++)
    for(;;)
    {
        int random_number = rand() % 1000; // Zufallszahl zwischen 0 und 999

        snprintf((char *)shm_ptr, SHM_SIZE, "%d", random_number);
        printf("Prozess 1 gespeichert: %d\n", random_number);
        struct timespec ts = {0, 100000000}; // 1 Millisekunde
        nanosleep(&ts, NULL);
    }

    // 5. Shared Memory entmappen und schließen
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
}