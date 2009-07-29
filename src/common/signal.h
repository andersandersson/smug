#ifndef SMUG_COMMON_SIGNAL_H
#define SMUG_COMMON_SIGNAL_H

#include "common.h"

#define SIG_EXIT  0x0001 // Tell engine to stop
#define SIG_TERM  0x0002 // Tell all systems to shut down

int Signal_init();
void Signal_terminate();
void Signal_send(int signal);
BOOL Signal_check(int signal);
void Signal_clear(int signal);

#endif // SMUG_COMMON_SIGNAL_H
