#include "interface.h"
#include "log.h"

void smugLogSetLevel(int level)
{
    Log_setLevel(level);
}

void smugLogPrint(char* msg)
{
    Log_printLine(msg);
}
