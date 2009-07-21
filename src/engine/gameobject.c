#include <stdlib.h>
#include "gameobject.h"

GameObject* GameObject_new()
{
	GameObject* go = (GameObject*)malloc(sizeof(GameObject));
	
	go->x = 0.0f;
	go->y = 0.0f;
	
	go->shape = NULL;
	go->sprite = NULL;
	
	return go;
}

void GameObject_delete(void* gameObject)
{
	if (NULL != gameObject)
	{
		free((GameObject*)gameObject);
	}
}
