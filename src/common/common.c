#include "common.h"

#include "stdio.h"

#ifndef DISABLE_ASSERTS
void Smug_assert(BOOL result, char* file, int line)
{
	if(TRUE != result)
	{
		fprintf(stderr, "Assertion failed at line %d in file %s\n", line, file);
	}
}
#endif
