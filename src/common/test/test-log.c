#include "log.h"

int main()
{
    Log_init();

    Log_printLine("Anders leker");
    Log_indent();
    Log_pushPrefix(": ");
    Log_printLine("Anders leker");
    Log_printLine("Anders leker");
    Log_indent();
    Log_printLine("Anders leker");
    Log_printLine("Anders leker");
    Log_dedent();
    Log_dedent();
    Log_popPrefix();
    Log_printLine("Anders leker");

    Log_setLevel(LOG_ALL);

    DEBUG("Debugtest %d", 123);
    NOTIFY("Notice: %f", 12.123);
    WARNING("Warning: %s", "some warning");
    ERROR("Error: {{%-20s}}", "more error");

    Log_terminate();
    return 0;
}
