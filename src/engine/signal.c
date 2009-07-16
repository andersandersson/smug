#include "signal.h"


static int gSignals = 0;


void Signal_send(int signal)
{
   gSignals = gSignals | signal;
}


char Signal_check(int signal)
{
   if( signal == (gSignals & signal) )
      {
         return TRUE;
      }
   
   return FALSE;
}

