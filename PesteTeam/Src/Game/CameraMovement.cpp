#include "CameraMovement.h"
#include <GameObject.h>
CameraMovement::CameraMovement(GameObject * camera_, GameObject* _target, GameObject* _pivot) :BehaviourComponent(camera_),target(_target), pivot(_pivot)
{
	keyboard = MainApp::instance()->getKeyboard();
}

CameraMovement::~CameraMovement()
{
}


void CameraMovement::Update(float t)
{

	if (keyboard->isKeyDown(OIS::KC_A)) {
		if (gameObject->getPosition().x > -30) {
			gameObject->translate(Vec3(-30 * t, 0, 0));
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		if (gameObject->getPosition().x < 30) {
			gameObject->translate(Vec3(30 * t, 0, 0));
		}

	}
	else {
		if (gameObject->getPosition().x > 1) {
			gameObject->translate(Vec3(-20 * t, 0,0));
		}
		else if (gameObject->getPosition().x < -1) {
			gameObject->translate(Vec3(20 * t, 0,0));
		}
	}
	if (keyboard->isKeyDown(OIS::KC_W)) {
		if (gameObject->getPosition().y > -10) {
			gameObject->translate(Vec3(0, -40 * t, 0));
		}
		if (pivot->getPosition().y < 10){
			pivot->translate(Vec3(0, 40 * t, 0));
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_S)){

		if (gameObject->getPosition().y < 20){
			gameObject->translate(Vec3(0, 40 * t, 0));
		}
		if (pivot->getPosition().y > -10) {
			pivot->translate(Vec3(0,-40 * t, 0));
		}

	}

	Euler e;
	e.fromQuaternion(gameObject->getOrientation());
	e.mRoll = 0;
	gameObject->setOrientation(e);
	gameObject->lookAt(pivot->getGlobalPosition());

}

void CameraMovement::reciveMsg(Message * msg)
{

}