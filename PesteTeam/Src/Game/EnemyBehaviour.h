#pragma once
#include <BehaviourComponent.h>

class EnemyBehaviour : public BehaviourComponent
{
public:
	EnemyBehaviour(GameObject* gameObject);
	virtual ~EnemyBehaviour();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
};

