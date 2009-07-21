/* -*- tab-width: 4 -*- */ 

#include "common/common.h"
#include "engine/engine.h"
#include "common/log.h"

#include "stdio.h"

int main(char argc, char* argv[])
{
	if (argc != 2)
	{
		ERROR("Please provide script file\n", "fult");
		return 0;	
	}
	
	if (!Script_init(argv[1]))
	{
		ERROR("Could not initialize script, exiting...\n", "fulare");
		Script_terminate();
		return 0;
	}

	Script_run();
	Script_terminate();
	
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
