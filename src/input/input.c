#include "input.h"

#include "utils/arraylist.h
#include "utils/linkedlist.h"


static ArrayList* devices;



static void assertDevice(unsigned int device)
{
	ArrayList* buttons = ArrayList_get(devices, device);
	if (buttons == NULL)
	{
	
	
	
	}


}



int Input_init(int width, int height);
{



	return 1;
}

void Input_terminate()
{




}



void Input_addKeyHook(int key, Hook* hook)
{
	addHook(DEVICE_KEYBOARD, key, hook);

}

void Input_addMouseMoveHook(Hook* hook)
{
	addHook(DEVICE_KEYBOARD, key, hook);

}

void Input_addMouseButtonHook(int key, Hoo* hook)
{
	addHook(DEVICE_KEYBOARD, key, hook);	

}


