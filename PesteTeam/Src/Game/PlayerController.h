#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
#include "Messages.h"

class PlayerController :
	public BehaviourComponent
{
private:
	float pi = 3.14159265359;
	bool moved = false;
	Euler euler;
	OIS::Keyboard* keyboard;
	float lastT = 0;
	Euler e;
public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
}; 

