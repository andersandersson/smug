#include "signal.h"

static int gSignals = 0;


void Signal_Send(int signal)
{
   gSignals = gSignals | signal;
}


BOOL Signal_Check(int signal)
{
   if( signal == (gSignals & signal) )
      {
         return TRUE;
      }
   
   return FALSE;
}

