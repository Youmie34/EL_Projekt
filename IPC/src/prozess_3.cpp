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
#include "../header/prozess_3.h"

int message[SIZE_MESSAGE];

void decimalToBinary(int *array, int startIndex, int decimalNumber)
{
    for (int i = 0; i < 8; i++)
    {
        array[startIndex + i] = (decimalNumber >> (7 - i)) & 1;
    }
}

unsigned int calculateCRC(int *message, int length)
{
    unsigned int crc = 0xFFFFFFFF;
    for (int i = 0; i < length; i++)
    {
        crc ^= message[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFF;
}

void init_message(int *shm_ptr, int *shm_ptr2)
{
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

    int sensorValue1 = readSensorValue(SHM_Sensor_1);
    if (sensorValue1 != -1)
    {
        printf("Gelesene Daten 1: %d\n", shm_ptr);
        decimalToBinary(message, 12, sensorValue1);
    }

    // Wert-ID 1 4 Bit
    message[20] = 0;
    message[21] = 0;
    message[22] = 1;
    message[23] = 0;

    int sensorValue2 = readSensorValue(SHM_Sensor_2);
    if (sensorValue2 != -1)
    {
        printf("Gelesene Daten 2: %d\n", shm_ptr2);
        decimalToBinary(message, 24, sensorValue2);
    }

    // Ende der Sensordaten ist bei message[31]
    // TODO: add CRC, CRC starts at message[32]
    unsigned int crc = calculateCRC(message, 32);
    for (int i = 0; i < 32; i++)
    {
        message[32 + i] = (crc >> (31 - i)) & 1;
    }
}

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

    // 2. Shared Memory in den Adressraum mappen (Prozess 1)
    int *shm_ptr = (int *)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    // 2. Shared Memory in den Adressraum mappen (Prozess 2)
    int *shm_ptr2 = (int *)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd2, 0);
    if (shm_ptr2 == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    // sem_post(sem);

    // TODO: Message inistialisieren
    init_message(shm_ptr, shm_ptr2);

    // message an gpio_control weiterleiten
    control_gpios(message, SIZE_MESSAGE);

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