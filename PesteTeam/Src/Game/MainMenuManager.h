#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>
#include "Messages.h"
class MainMenuManager :
	public BehaviourComponent
{
private:
	//Struct para guardar la configuración inicial de los botones
	struct ButtonProps {
		string name; int x; int y; int w; int h;
		ButtonProps(string name) : name(name) {
			x = GUIManager::instance()->getImage(name)->getPosition().left;
			y = GUIManager::instance()->getImage(name)->getPosition().top;
			w = GUIManager::instance()->getImage(name)->getWidth();
			h = GUIManager::instance()->getImage(name)->getHeight();
		}
	};
	GUIManager* GUIMgr = nullptr;
	//States
	int state = 0;// 0=Play  1=Credits  2=Exit
	void handleStates();
	//Buffer
	OIS::KeyCode lastKey;
	double inputBufferTimer=0.2;
	double lastTimePressed=0;
	//Animations
	MyGUI::ImageBox* titleImage = nullptr;
	int titleWidth=0;
	int titleHeight=0;
	int titleX = 0;
	int titleY = 0;
	int titleAmplitude = 0;
	float titleSinPeriod = 0;
	MyGUI::ImageBox* activeButton = nullptr;
	ButtonProps playProps;
	ButtonProps creditsProps;
	ButtonProps exitProps;
	ButtonProps activeProps;
	int buttonAmplitude = 0;
	float buttonSinPeriod = 0;

	void titleAnimation();
	void buttonAnimation();
public:
	MainMenuManager(GameObject* gameObject);
	virtual ~MainMenuManager();

	OIS::Keyboard* keyboard;
	void setButtonSinPeriod(float sinP);
	void setButtonAmplitude(float amplitude);
	void setTitleSinPeriod(float sinP);
	void setTitleAmplitude(float amplitude);
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

