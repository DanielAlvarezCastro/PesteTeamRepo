#include <Ogre.h>
#include <vector>
#include "RigidBody.h"
#include "BehaviourComponent.h"
#include "MainApp.h"

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

class GameObject
{
private:
	struct Transform
	{
#pragma region Atributes
		Ogre::Vector3 position; //Posicion en el espacio
		Ogre::Vector3 direction; //Direccion de la entidad
		Ogre::Vector3 rotation; //Rotacion de la entidad 
		Ogre::Vector3 scale; //Tamaño del objeto
#pragma endregion
		//Inializaciones por defecto del trasform
		Transform(Ogre::Vector3 position_= Ogre::Vector3::ZERO, Ogre::Vector3 direction_ = Ogre::Vector3::ZERO, 
			Ogre::Vector3 rotation_ = Ogre::Vector3::ZERO, Ogre::Vector3 scale_ = Ogre::Vector3::ZERO):
			position(position_), direction(direction_), rotation(rotation_), scale(scale_) {};

	}transform;
	GameObject* father = NULL;
	RigidBody* rigidBody;
	std::vector<BehaviourComponent*>behaviourComponents;
	//scena en la que se encuentra
	int references = 0;
	bool active = true;
	//cosas de Ogre
	Ogre::SceneNode* ogreNode = nullptr;
	Ogre::Entity* ogreEntity = nullptr;

public:
	GameObject();
	~GameObject();
	void addReference() { references++; };
	void release();

#pragma region Transform functions
#pragma region Trasform Getters
	Ogre::Vector3 getPosition(){ return transform.position; };
	Ogre::Vector3 getDirection() { return transform.direction; };
	Ogre::Vector3 getRotation(){ return transform.rotation; };
	Ogre::Vector3 getScale() { return transform.scale; };
#pragma endregion
#pragma region Transform Setters
	void setPosition(Ogre::Vector3 position_) { transform.position = position_; ogreNode->setPosition(transform.position); };
	void setDirection(Ogre::Vector3 direction_) { transform.direction = direction_; ogreNode->setDirection(transform.direction); };
	void setRotation(Ogre::Vector3 rotation_) { transform.rotation = rotation_; };
	void setScale(Ogre::Vector3 scale_) { transform.scale = scale_; ogreNode->setScale(transform.scale); };
#pragma endregion
#pragma endregion
	void setActive(bool active_) { active = active_; };
	void reciveMsg(Message* msg);
	void asingFather(GameObject* father_) { father = father_; };
	void createEntity(std::string mesh);
};

#endif // _GAMEOBJECT



