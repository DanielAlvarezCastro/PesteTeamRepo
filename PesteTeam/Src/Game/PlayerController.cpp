#include "PlayerController.h"



PlayerController::PlayerController(GameObject* gameObject):BehaviourComponent(gameObject)
{
	keyboard = MainApp::instance()->getKeyboard();
}


PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	if (keyboard->isKeyDown(OIS::KC_Q)) {
		MainApp::instance()->closeApp();
	}
}
