#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"
#include <GUIManager.h>
class GameGUI :
	public BehaviourComponent
{
private:
	int healthbarFullW = 400;
	int fullHealth = 200;
	GUIManager* GUIMgr = nullptr;
	MyGUI::ImageBox* healthBar = nullptr;
	MyGUI::ImageBox* gameOver = nullptr;
	MyGUI::ImageBox* missionA = nullptr;
	MyGUI::ImageBox* warning = nullptr;
public:
	GameGUI(GameObject* gameObject);
	virtual ~GameGUI();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

