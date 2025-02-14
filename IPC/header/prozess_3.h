/**
 * @file prozess_3.h
 * @author your name (you@domain.com)
 * @brief This program reads the sensor data from the two processes and writes them in the message array
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __Prozess_3__
#define __Prozess_3__

#include "helper.h"
#include "gpio_control.h"

#define SIZE_MESSAGE 40

// Sebastian
void prozess3();
// Caroline
void init_message(int *shm_ptr, int *shm_ptr2);
void decimalToBinary(uint8_t *array, int startIndex, int decimalNumber);
// Justin
unsigned int calculateCRC(int *message, int length);

extern int message[];

#endif /*__Prozess_3__*/