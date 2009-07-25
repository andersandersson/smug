#include "signal.h"

static int gSignals = 0;
#include "stdio.h"

void Signal_send(int signal)
{
   gSignals = gSignals | signal;
}

BOOL Signal_check(int signal)
{
   if(signal == (gSignals & signal))
   {
         return TRUE;
   }
   
   return FALSE;
}

BOOL Signal_checkAndClear(int signal)
{ 
    if(signal == (gSignals & signal))
    {
        gSignals = (gSignals & !signal);
        return TRUE;
    }
   
    return FALSE;
}
