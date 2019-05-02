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
	Scene* scn;

	int bulletCount = 0;
	int cooldown = 1;
	bool keyDown = false;
	std::string shipName_;
	std::string bulletMeshName;

	std::vector<GameObject*> bullets_;
	std::vector<BulletBehaviour*> bComponents_;

public:
	ShotBehaviour(GameObject* gameObject, std::string shipName);
	virtual ~ShotBehaviour();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};

	void getBullets();
	void Shoot();
	void UpdateValues(int i, std::pair<GameObject*, GameObject*> blls);
};

