#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // Für O_* Konstanten
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "../header/prozess_1.h"

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
    int *shm_ptr = (int *)mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // 4. Daten schreiben

    srand(time(NULL));

    // for (int i = 0; i < 10; i++)
    for (;;)
    {

        int random_number = rand() % 256; // Zufallszahl zwischen 0 und 255
        // CHANGED
        *shm_ptr = random_number; // Direkt als Integer schreiben
        // snprintf((char *)shm_ptr, SHM_SIZE, "%d", random_number);
        printf("Prozess 1 gespeichert: %d\n", random_number);
        struct timespec ts = {0, 100000000}; // 1 Millisekunde
        nanosleep(&ts, NULL);
    }

    // 5. Shared Memory entmappen und schließen
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
}