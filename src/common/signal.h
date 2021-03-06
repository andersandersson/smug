/**
 * @file signal.h
 * @brief Header file for signaling system
 * @if doc_files
 * @ingroup smug_common
 * @endif
 */

/**
 * @addtogroup smug_common
 * @{
 */

#ifndef SMUG_COMMON_SIGNAL_H
#define SMUG_COMMON_SIGNAL_H

#include <common/common.h>

#define SIG_EXIT  0x0001 // Tell engine to stop
#define SIG_TERM  0x0002 // Tell all systems to shut down

int Signal_init(void);
BOOL Signal_isInitialized(void);
void Signal_terminate(void);
void Signal_send(int signal);
BOOL Signal_check(int signal);
void Signal_clear(int signal);

#endif // SMUG_COMMON_SIGNAL_H

/**@}*/
