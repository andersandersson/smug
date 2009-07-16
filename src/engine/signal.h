#ifndef SIGNAL_H
#define SIGNAL_H

#include "common.h"

#define SIG_EXIT  0x0001  // Tell all systems to shutdown

void Signal_send(int signal);
BOOL Signal_check(int signal);

#endif // SIGNAL_H
