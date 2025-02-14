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

extern int message[];

// TODO: add CRC

#endif /*__Prozess_3__*/