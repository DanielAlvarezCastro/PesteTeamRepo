#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
class PlayerController :
	public BehaviourComponent
{
public:
	PlayerController(GameObject* gameObject);
	virtual ~PlayerController();

	OIS::Keyboard* keyboard;

	virtual void Update();
	virtual void reciveMsg(Message* msg) {};
}; 

