#pragma once
#include <BehaviourComponent.h>

class EnemyBehaviour : public BehaviourComponent
{
private:
	int life;

public:
	EnemyBehaviour(GameObject* gameObject, int l = 20);
	virtual ~EnemyBehaviour();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};

	void hit(int value) { life -= value; }
};

