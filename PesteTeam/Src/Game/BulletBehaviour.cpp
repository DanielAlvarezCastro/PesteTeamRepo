#include "BulletBehaviour.h"
#include <GameObject.h>

BulletBehaviour::BulletBehaviour(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	pos_ = gameObject->getPosition();
	dir_ = gameObject->getDirection();
	dir_.normalise();
}

BulletBehaviour::~BulletBehaviour()
{

}

void BulletBehaviour::Update(float t)
{
	pos_ += dir_ * 15;
	gameObject->setPosition(pos_);
}