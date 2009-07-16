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

/*! @mainpage Smug - Scripted Multithreaded Utility for Games
 *
 * @section intro_sec Introduction
 *
 * This is the introduction.
 *
 * @section install_sec Installation
 *
 * @subsection step1 Step 1: Opening the box
 *  
 * etc...
 */
