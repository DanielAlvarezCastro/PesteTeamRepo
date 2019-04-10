#include "MainMenuManager.h"
#include <GameObject.h>



MainMenuManager::MainMenuManager(GameObject* gameObject): BehaviourComponent(gameObject)
{
	keyboard = MainApp::instance()->getKeyboard();
}


MainMenuManager::~MainMenuManager()
{
}

void MainMenuManager::Update(float t)
{
	//Timer que regula la velocidad de input
	if (lastKey == OIS::KC_W || lastKey == OIS::KC_S) {
		lastTimePressed += t;
		if (lastTimePressed > inputBufferTimer) {
			lastKey = OIS::KC_U;
			lastTimePressed = 0;
		}
	}
	if (keyboard->isKeyDown(OIS::KC_W) && lastKey!= OIS::KC_W) {
		state--;
		handleStates();
		lastKey = OIS::KC_W;
	}
	else if (keyboard->isKeyDown(OIS::KC_S) && lastKey != OIS::KC_S) {
		state++;
		handleStates();
		lastKey = OIS::KC_S;
	}
	
}
void MainMenuManager::handleStates()
{
	if (state < 0) state = 2;
	else if (state > 2) state = 0;
	if (state == 0) //PLAY
	{
		GUIManager::instance()->getImage("Play")->setImageTexture("PlaySelected.png");
		GUIManager::instance()->getImage("Credits")->setImageTexture("CreditsUnSelected.png");
		GUIManager::instance()->getImage("Exit")->setImageTexture("ExitUnSelected.png");
	}
	else if(state == 1) //CREDITS
	{
		GUIManager::instance()->getImage("Play")->setImageTexture("PlayUnSelected.png");
		GUIManager::instance()->getImage("Credits")->setImageTexture("CreditsSelected.png");
		GUIManager::instance()->getImage("Exit")->setImageTexture("ExitUnSelected.png");
	}
	else if (state == 2) //EXIT
	{
		GUIManager::instance()->getImage("Play")->setImageTexture("PlayUnSelected.png");
		GUIManager::instance()->getImage("Credits")->setImageTexture("CreditsUnSelected.png");
		GUIManager::instance()->getImage("Exit")->setImageTexture("ExitSelected.png");
	}
}


void MainMenuManager::reciveMsg(Message * msg)
{
}
