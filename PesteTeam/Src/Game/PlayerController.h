#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
class PlayerController :
	public BehaviourComponent
{
public:
	PlayerController(GameObject* gameObject, GameObject* gameObject2_);
	virtual ~PlayerController();

	float pi = 3.14159265359;
	GameObject* pointer;
	OIS::Keyboard* keyboard;

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
}; 

