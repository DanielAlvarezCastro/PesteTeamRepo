#include "EnemyBehaviour.h"
#include <GameObject.h>

EnemyBehaviour::EnemyBehaviour(GameObject* gameObject, int l) : BehaviourComponent(gameObject), life(l)
{
}

EnemyBehaviour::~EnemyBehaviour()
{
}

void EnemyBehaviour::Update(float t) 
{
	if (life <= 0)
		this->gameObject->setActive(false);
}