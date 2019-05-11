#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
class ShipController : public BehaviourComponent
{
private:
	Euler euler;
	Ogre::Radian iniOrientation;
	OIS::Keyboard* keyboard;
	float pi = 3.14159265359;
	int health;
	bool isRolling = false;
	bool rollRight = false;
	bool rollLeft = false;
	Real rollVel = pi / 2;
	Real rollBackVel = pi / 4;
	Real barrelVel = 5*pi;
	bool warningZone = false;
public:
	ShipController(GameObject* gameObject, int _health);
	virtual ~ShipController();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};