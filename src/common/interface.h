#ifndef SMUG_COMMON_INTERFACE_H
#define SMUG_COMMON_INTERFACE_H

// Should this file really exist?

#define SMUG_LOG_LEVEL_DEBUG 		   0x01
#define SMUG_LOG_LEVEL_WARNING         0x02
#define SMUG_LOG_LEVEL_ERROR           0x04
#define SMUG_LOG_LEVEL_NOTIFICATION    0x08
#define SMUG_LOG_LEVEL_ALL             0xFF

void smugLogSetLevel(int level);

void smugLogPrint(char* msg);

/* void smugLog(int level, char* fmt, ...); */

#endif // SMUG_COMMON_INTERFACE_H
