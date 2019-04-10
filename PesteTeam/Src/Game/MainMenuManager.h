#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>
#include "Messages.h"
class MainMenuManager :
	public BehaviourComponent
{
private:
	int state = 0;// 0=Play  1=Credits  2=Exit
	void handleStates();
	OIS::KeyCode lastKey;
	double inputBufferTimer=0.2;
	double lastTimePressed=0;
public:
	MainMenuManager(GameObject* gameObject);
	virtual ~MainMenuManager();

	OIS::Keyboard* keyboard;

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

