#include "EnemyBehaviour.h"
#include <GameObject.h>
#include "Messages.h"
EnemyBehaviour::EnemyBehaviour(GameObject* gameObject, int l) : BehaviourComponent(gameObject), life(l)
{
}

EnemyBehaviour::~EnemyBehaviour()
{
}

void EnemyBehaviour::Update(float t) 
{
	if (life <= 0) {
		EnemyDeadMsg msg;
		sendSceneMsg(&msg);
		this->gameObject->setActive(false);
	}
}