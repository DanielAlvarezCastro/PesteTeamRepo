#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
class ShipController :
	public BehaviourComponent
{
private:
	Euler euler;
	OIS::Keyboard* keyboard;
	float pi = 3.14159265359;
public:
	ShipController(GameObject* gameObject);
	virtual ~ShipController();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
};