#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Physics.h>
#include <SceneMachine.h>
#include "BulletBehaviour.h"
#include "Messages.h"

enum enemyType {
	groundTurret,
	Flyer
};

class EnemyShoot: public BehaviourComponent
{
protected:
	enemyType type;
	std::vector<GameObject*>bullets;
	std::vector<BulletBehaviour*> bulletComponents;
	std::string bulletMeshName;

	GameObject* target;

	float ShootCd = 1,
		auxTemp = 0;
	float range = 100;
public:
	EnemyShoot(GameObject* go, enemyType type_, GameObject* target_, std::string bulletMesh);
	virtual ~EnemyShoot();
	void changeRange(float nRange) { range = nRange; };
	void shoot();

	void situateBullet(GameObject*b, int id, bool created = false, int i = -1);
	
	void getBullet(int id);

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
};

