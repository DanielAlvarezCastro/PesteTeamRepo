#include <Ogre.h>
#include <vector>
#include "RigidBody.h"
#include "BehaviourComponent.h"
#include "MainApp.h"

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

struct Vec3 
{
	float x;
	float y;
	float z;

	Vec3(float _x=0, float _y=0, float _z=0) : x(_x), y(_y), z(_z){};
	Vec3(Ogre::Vector3 vec): x(vec.x), y(vec.y), z(vec.z) {};
	Ogre::Vector3 getVector() { return Ogre::Vector3((Ogre::Real)x, (Ogre::Real)y, (Ogre::Real)z); };
};

class GameObject
{
private:
	struct Transform
	{
#pragma region Atributes
		Ogre::Vector3 position; //Posicion en el espacio
		Ogre::Vector3 direction; //Direccion de la entidad
		Ogre::Vector3 scale; //Tamaño del objeto
#pragma endregion
		//Inializaciones por defecto del trasform
		Transform(Ogre::Vector3 position_ = Ogre::Vector3::ZERO, Ogre::Vector3 direction_ = Ogre::Vector3::ZERO,
			Ogre::Vector3 rotation_ = Ogre::Vector3::ZERO, Ogre::Vector3 scale_ = Ogre::Vector3::ZERO) :
			position(position_), direction(direction_), scale(scale_) {};

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
	Vec3 getPosition() { return Vec3(transform.position); };
	Vec3 getDirection() { return Vec3(transform.direction); };
	Vec3 getScale() { return Vec3(transform.scale); };
#pragma endregion
#pragma region Transform Setters
	void setPosition(Vec3 position_) {transform.position = position_.getVector(); ogreNode->setPosition(transform.position); };
	void setDirection(Vec3 direction_) { transform.direction = direction_.getVector(); ogreNode->setDirection(transform.direction); };
	void setScale(Vec3 scale_) { transform.scale = scale_.getVector(); ogreNode->setScale(transform.scale); };
	void setInitialState() { ogreNode->setInitialState(); };
#pragma endregion
#pragma region Position Metods
	void translate(Vec3 movement) { ogreNode->translate(movement.getVector()); setPosition(ogreNode->getPosition()); };
#pragma endregion
#pragma region Rotation Metods
	void yaw(float radian) { ogreNode->yaw(Ogre::Radian((Ogre::Real)radian)); setDirection(ogreNode->getOrientation()*Ogre::Vector3::UNIT_Z); };
	void pitch(float radian) { ogreNode->pitch(Ogre::Radian((Ogre::Real)radian)); setDirection(ogreNode->getOrientation()*Ogre::Vector3::UNIT_Z); };
	void roll(float radian) { ogreNode->roll(Ogre::Radian((Ogre::Real)radian)); setDirection(ogreNode->getOrientation()*Ogre::Vector3::UNIT_Z); };
	void rotate(Vec3 axis, float radian) { ogreNode->rotate(axis.getVector(), Ogre::Radian((Ogre::Real)radian)); setDirection(ogreNode->getOrientation()*Ogre::Vector3::UNIT_Z); };
	void lookAt(Vec3 position, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_WORLD) {
		ogreNode->lookAt(position.getVector(), relativeTo); setDirection(ogreNode->getOrientation()*Ogre::Vector3::UNIT_Z);
	}
#pragma endregion
#pragma endregion
	void setActive(bool active_) { active = active_; };
	void reciveMsg(Message* msg);
	void asingFather(GameObject* father_) { father = father_; };
	void createEntity(std::string mesh);
};

#endif // _GAMEOBJECT



