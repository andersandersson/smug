#include "common.h"

#include "stdio.h"

#ifndef DISABLE_ASSERTS
void Smug_assert(BOOL result, int line)
{
	if(TRUE != result)
	{
		fprintf(stderr, "Assertion failed at line %d", line);
	}
}
#endif
