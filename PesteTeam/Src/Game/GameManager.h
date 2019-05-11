#pragma once
#include <BehaviourComponent.h>
class GameManager : public BehaviourComponent
{
private:
	bool GameOver;
	bool nextLevel;
	static int currentLevel;
	float nextLevelMaxTime;
	float nextLevelTimer;
public:
	GameManager(GameObject* go);
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
	virtual ~GameManager();
};

