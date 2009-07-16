/* -*- tab-width: 4 -*- */ 

#include "engine/engine.h"

int main(char argc, char* argv[])
{
	if (Engine_init())
	{
		Engine_run();
	}
	
	Engine_terminate();
	return 0;
}
