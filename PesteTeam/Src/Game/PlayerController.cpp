#include "PlayerController.h"
#include <GameObject.h>



PlayerController::PlayerController(GameObject* gameObject, GameObject* gameObject2_):BehaviourComponent(gameObject), pointer(gameObject2_)
{
	keyboard = MainApp::instance()->getKeyboard();
}


PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	if (keyboard->isKeyDown(OIS::KC_A)) {
		pointer->translate(Vec3(-0.5, 0, 0));
		gameObject->lookAt(pointer->getPosition());
		if (gameObject->getRoll() < pi / 4) { gameObject->roll(0.02); }
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		pointer->translate(Vec3(0.5, 0, 0));
		gameObject->lookAt(pointer->getPosition());
		if (gameObject->getRoll() > -pi / 4) { gameObject->roll(-0.02); }
	}
	else 
	{
		if (gameObject->getRoll() > 0) 
		{
			gameObject->roll(-0.01);
		}
		else if (gameObject->getRoll() < 0) 
		{
			gameObject->roll(0.01);
		}
	}
	if (keyboard->isKeyDown(OIS::KC_W)) {
		pointer->translate(Vec3(0, -0.5, 0));
		gameObject->lookAt(pointer->getPosition());
	}
	else if (keyboard->isKeyDown(OIS::KC_S)) {
		pointer->translate(Vec3(0, 0.5, 0));
		gameObject->lookAt(pointer->getPosition());
	}
	if (keyboard->isKeyDown(OIS::KC_Q)) {
		gameObject->yaw(0.01);
	}
	else if (keyboard->isKeyDown(OIS::KC_E)) {
		gameObject->yaw(-0.01);
	}
	if (keyboard->isKeyDown(OIS::KC_SPACE)) {
 		gameObject->translate(gameObject->getDirection());
		pointer->translate(gameObject->getDirection());
	}
}
