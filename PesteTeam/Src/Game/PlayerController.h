#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
#include "Messages.h"

class PlayerController : public BehaviourComponent
{
private:
	float pi = 3.14159265359;
	bool moved = false;
	Euler euler;
	OIS::Keyboard* keyboard;
	float lastT = 0;
	Euler e;
	Real rotationVel = pi / 4;//How fast ship rotates
	Real pitchLimit = pi / 7;//Look up/down limit
	Real slowSpeed = 30;
	Real normalSpeed = 60;
	Real fastSpeed = 120;

public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
}; 

