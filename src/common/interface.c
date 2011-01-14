#include <common/log.h>
#include <common/interface.h>

static int gLevel;

static char* _getInternalLevel(SmugLogLevel level)
{
    switch (level)
    {
        case SMUG_LOG_LEVEL_ERROR:
            gLevel = LOG_ERROR;
            return "ERROR";
        case SMUG_LOG_LEVEL_WARNING:
            gLevel = LOG_WARNING;
            return "WARNING";
        case SMUG_LOG_LEVEL_NOTIFICATION:
            gLevel = LOG_NOTIFICATION;
            return "NOTICE";
        case SMUG_LOG_LEVEL_DEBUG:
            gLevel = LOG_DEBUG;
            return "DEBUG";
        case SMUG_LOG_LEVEL_ALL:
            gLevel = LOG_ALL;
            return "";
        case SMUG_LOG_LEVEL_NONE:
            gLevel = LOG_NONE;
            return "";
        default:
            gLevel = LOG_DEBUG;
            return "DEBUG";
    }
}

void smugLogSetLevel(SmugLogLevel level)
{
    _getInternalLevel(level);
    Log_setLevel(gLevel);
}

void smugLogPrint(char* msg)
{
    Log_printLine(msg);
}

void smugLog(SmugLogLevel level, char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char* pref = _getInternalLevel(level);
    Log_addEntryVa(gLevel, pref, "??", 0, fmt, args);
    va_end(args);
}
