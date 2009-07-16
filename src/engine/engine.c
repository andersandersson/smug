#include "engine.h"

#include "stdio.h"

#include "GL/glfw.h"
#include "signal.h"
#include "console.h"
#include "platform/platform.h"
#include "graphics/graphics.h"
#include "physics/physics.h"

Thread* gConsoleThread = NULL;

static BOOL gInitialized = FALSE; 

int Engine_init()
{
	if (!glfwInit())
		return 0;

	if (!Graphics_init(640, 480, FALSE))
		return 0;
		
	if (!Physics_init())
		return 0;
	
	// Create a new thread for the console
	gConsoleThread = Thread_new();

	// Run the console main loop in a new thread
	Thread_call(gConsoleThread, Console_run, NULL);	

	gInitialized = TRUE;
	return 1;
}

void Engine_terminate()
{
	Physics_terminate();
	
	Graphics_terminate();
	
	glfwTerminate();
	
	// Do not wait for console thread to end as getc() will
	// block until input is received. Instead, kill the thread.
	glfwDestroyThread(gConsoleThread->id);	
	
	gInitialized = FALSE;
}

void Engine_run()
{
	if (!gInitialized)
		return;
		
	double lastFpsCheck = glfwGetTime();	
	int fps = 0;
	
	// Setup timing variables
	Time delay = 1.0f/30.0f;
	Time nexttime;
	Time time;
	nexttime = glfwGetTime();
	
	while(TRUE != Signal_check(SIG_EXIT))
	{
		time = glfwGetTime();
		
		if (time >= nexttime)
		{
			nexttime+=delay;
		
			if(glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ))
			{
				Signal_send(SIG_EXIT);
			}
		
			Graphics_refresh();	
			fps++;
		}
		
		if (glfwGetTime() - lastFpsCheck >= 1.0)
		{
			fprintf(stderr, "Fps: %i\n", fps);
			fps = 0;
			lastFpsCheck = glfwGetTime();
		}
		
		// TODO: add if-case: if nexttime-time > smallest_sleep_time
		glfwSleep(nexttime - time);
	}
}
