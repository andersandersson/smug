#include <common/log.h>
#include <common/interface.h>

void smugLogSetLevel(int level)
{
    Log_setLevel(level);
}

void smugLogPrint(char* msg)
{
    Log_printLine(msg);
}
