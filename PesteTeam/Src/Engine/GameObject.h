#include <Ogre.h>

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

using namespace Ogre;
class GameObject
{
private:
	struct Transform
	{
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;

		//Inializaciones por defecto del trasform
		Transform() {};
	}transform;
public:
	GameObject();
	~GameObject();
};

#endif // _GAMEOBJECT



