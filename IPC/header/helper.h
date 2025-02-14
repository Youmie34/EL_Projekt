#ifndef __helper__
#define __helper__

#include <unistd.h>
#include <time.h>

#define SHM_SIZE 1024
#define SHM_Sensor_1 "/shared_memory_Sensor_1"
#define SHM_Sensor_2 "/shared_memory_Sensor_2"
#define SEM_Prozess_1 "/SEM_Prozess_1"
#define SEM_Prozess_2 "/SEM_Prozess_2"

int readSensorValue(const char *shm_name);

#endif /*__helper__*/