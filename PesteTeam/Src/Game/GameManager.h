#pragma once
#include <BehaviourComponent.h>
class GameManager : public BehaviourComponent
{
private:
	bool GameOver;
	bool missionA;
	int currentLevel;
public:
	GameManager(GameObject* go);
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
	virtual ~GameManager();
};

