#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
#include "Messages.h"
class CameraMovement :
	public BehaviourComponent
{
public:
	CameraMovement(GameObject* camera_, GameObject* _target, GameObject* _pivot);
	virtual ~CameraMovement();

	OIS::Keyboard* keyboard;
	GameObject* pivot;
	GameObject* target;

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};