#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
class CameraMovement :
	public BehaviourComponent
{
public:
	CameraMovement(GameObject* camera_, GameObject* target_);
	virtual ~CameraMovement();
	GameObject* camera;
	GameObject* target;


	virtual void Update();
	virtual void reciveMsg(Message* msg) {};
};

