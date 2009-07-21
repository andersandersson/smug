#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

typedef struct GameObject
{
	float x;
	float y;
	
	void* shape;
	void* sprite;

} GameObject;

GameObject* GameObject_new();
void GameObject_delete(void* obj);

#endif //GAMEOBJECT_H
