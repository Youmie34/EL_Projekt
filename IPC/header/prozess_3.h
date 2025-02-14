#ifndef __Prozess_3__
#define __Prozess_3__

#include "helper.h"
#include "gpio_control.h"

#define SIZE_MESSAGE 40

void prozess3();
void init_message();
void decimalToBinary(uint8_t *array, int startIndex, int decimalNumber);

extern int message[];

// TODO: add CRC

#endif /*__Prozess_3__*/