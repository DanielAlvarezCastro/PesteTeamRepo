#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>s
#include <Euler.h>
class ShipController :
	public BehaviourComponent
{
private:
	Euler euler;
	Ogre::Radian iniOrientation;
	OIS::Keyboard* keyboard;
	float pi = 3.14159265359;
	bool isRolling = false;
	bool rollRight = false;
	bool rollLeft = false;
public:
	ShipController(GameObject* gameObject);
	virtual ~ShipController();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
};