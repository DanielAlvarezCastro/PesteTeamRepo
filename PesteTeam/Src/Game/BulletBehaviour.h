#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"

class BulletBehaviour : public BehaviourComponent
{	
private:
	Ogre::Vector3 dir_;
	Ogre::Vector3 pos_;
public:
	BulletBehaviour(GameObject* gameObject);
	virtual ~BulletBehaviour();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
};

