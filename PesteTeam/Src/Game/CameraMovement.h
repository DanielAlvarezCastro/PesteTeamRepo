#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
#include "Messages.h"
class CameraMovement :
	public BehaviourComponent
{
private:
	Real horizontalLimit = 30;
	Real horizontaVel = 1;
	Real horizontalBackVel = 20;
	Real verticalLimit = 10;
	Real verticalVel = 4;
	Real aceleratedCameraOffset = 80;
	Real cameraDefaulOffset = 40;
public:
	CameraMovement(GameObject* camera_, GameObject* _target, GameObject* _pivot);
	virtual ~CameraMovement();

	OIS::Keyboard* keyboard;
	GameObject* pivot;
	GameObject* target;

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};