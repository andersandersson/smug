/* -*- tab-width: 4 -*- */ 

#include "GL/glfw.h"
#include "graphics/graphics.h"
#include "physics/physics.h"
#include "platform/platform.h"
#include "signal.h"
#include "console.h"
#include "log.h"

int main(char argc, char* argv[])
{
	Thread* console_thread;

	glfwInit();
	Graphics_Init(640, 480, 0);

	// Create a new thread for the console
	console_thread = Thread_New();

	// Run the console main loop in a new thread
	Thread_Call(console_thread, Console_Run, NULL);


	// Run main loop and wait for ESC or window close.
	while( TRUE != Signal_Check(SIG_EXIT) )
	{
		glfwSwapBuffers();

		if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ))
		{
			Signal_Send(SIG_EXIT);
		}
	}

	// Do not wait for console thread to end as getc() will
	// block until input is received. Instead, kill the thread.

	//Thread_Join(console_thread);
	//Thread_Delete(console_thread); 
	glfwDestroyThread(console_thread->id);

	glfwTerminate();
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
