#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"
#include <GUIManager.h>
class GameGUI :
	public BehaviourComponent
{
private:
	GUIManager* GUIMgr = nullptr;
	MyGUI::ImageBox* healthBar = nullptr;
	MyGUI::ImageBox* gameOver = nullptr;
	MyGUI::ImageBox* nextLevel = nullptr;
public:
	GameGUI(GameObject* gameObject);
	virtual ~GameGUI();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

