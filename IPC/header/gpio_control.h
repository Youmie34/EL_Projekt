#ifndef __GPIO_CONTROL__
#define __GPIO_CONTROL__

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "helper.h"
#include "libgpiod.h"
#include "prozess_3.h"

extern int senden;

struct thread_data
{
    int thread_num;
    gpiod_line_s *line;
};

void *thread_function(void *arg);
void *thread_function2(void *arg);
int control_gpios(int *message, int size);

#endif /*__GPIO_CONTROL__*/