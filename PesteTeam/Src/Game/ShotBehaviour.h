#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Physics.h>
#include <SceneMachine.h>
#include "BulletBehaviour.h"
#include "Messages.h"

class ShotBehaviour : public BehaviourComponent
{
private:
	OIS::Keyboard* keyboard;
	int bulletCount = 0;
	int cooldown = 50;
	bool keyDown = false;
	Scene* scn;

public:
	ShotBehaviour(GameObject* gameObject);
	virtual ~ShotBehaviour();

	virtual void Update(float t);
	void Shoot();
	virtual void reciveMsg(Message* msg) {};
};

