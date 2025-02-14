#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include "../header/helper.h"

int readSensorValue(const char *shm_name)
{
    int shm_fd = shm_open(shm_name, O_RDONLY, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        return -1;
    }

    int *shm_ptr = (int *)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        close(shm_fd);
        return -1;
    }

    int sensor_value = *shm_ptr;

    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    return sensor_value;
}

void sleep_custom(int sec, int nsec)
{
    // 1000 nano sec = 1 micro Sec
    struct timespec ts;
    ts.tv_sec = sec;   // Sekunden
    ts.tv_nsec = nsec; // Nanosekunden (10 Millisekunden = 10.000.000 Nanosekunden)
    nanosleep(&ts, NULL);
}