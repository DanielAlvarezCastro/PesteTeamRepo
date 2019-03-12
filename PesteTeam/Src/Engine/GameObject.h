#include <Ogre.h>
#include <vector>
#include "RigidBody.h"
#include "BehaviourComponent.h"

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

using namespace Ogre;
using namespace std;
class GameObject
{
private:
	struct Transform
	{
#pragma region Atributes
		Vector3 position; //Posicion en el espacio
		Vector3 rotation; //Rotacion de la entidad 
		Vector3 scale; //Tamaño del objeto
#pragma endregion
		//Inializaciones por defecto del trasform
		Transform(Vector3 position_= Vector3::ZERO, Vector3 rotation_ = Vector3::ZERO, Vector3 scale_ = Vector3::ZERO):
			position(position_), rotation(rotation_), scale(scale_) {};

	}transform;
	GameObject* father = NULL;
	RigidBody* rigidBody;
	std::vector<BehaviourComponent*>behaviourComponents;
	//scena en la que se encuentra
	int references = 0;
	bool active = true;
public:
	GameObject();
	~GameObject();
	void addReference() { references++; };
	void release();

#pragma region Transform functions
#pragma region Trasform Getters
	Vector3 getPosition(){ return transform.position; };
	Vector3 getRotation(){ return transform.rotation; };
	Vector3 getScale() { return transform.scale; };
#pragma endregion
#pragma region Trasnform Setters
	void setPosition(Vector3 position_) { transform.position = position_; };
	void setRotation(Vector3 rotation_) { transform.rotation = rotation_; };
	void setScale(Vector3 scale_) { transform.scale = scale_; };
#pragma endregion
#pragma endregion

	void setActive(bool active_) { active = active_; };
	void reciveMsg(Message* msg);
	void asingFather(GameObject* father_) { father = father_; };
};

#endif // _GAMEOBJECT



