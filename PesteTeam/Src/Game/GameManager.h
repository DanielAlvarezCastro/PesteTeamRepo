#pragma once
#include <BehaviourComponent.h>
class GameManager : public BehaviourComponent
{
private:
	bool GameOver;
	bool nextLevel;
	static int currentLevel;
	int maxLevel;
	float nextLevelWaitTime;
	float nextLevelTimer;
public:
	GameManager(GameObject* go, int _maxLevel, float _nextLevelWaitTime);
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
	virtual ~GameManager();
};

