#ifndef UPDATE_MAIN_H
#define UPDATE_MAIN_H

#include "common.h"
#include <cstdio>

// 1 hour delay, should always be under 24 hours
#define SHUTDOWN_DELAY (60 * 60)

int get_queue_length(void);
void get_entree(cat& entree);

#endif