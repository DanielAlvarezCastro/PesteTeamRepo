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
		if (gameObject->getPosition().x > -horizontalLimit) {
			gameObject->translate(Vec3(-horizontalLimit * horizontaVel * t, 0, 0));
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		if (gameObject->getPosition().x < horizontalLimit) {
			gameObject->translate(Vec3(horizontalLimit* horizontaVel * t, 0, 0));
		}

	}
	else {
		if (gameObject->getPosition().x > 1) {
			gameObject->translate(Vec3(-horizontalBackVel * t, 0,0));
		}
		else if (gameObject->getPosition().x < -1) {
			gameObject->translate(Vec3(horizontalBackVel * t, 0,0));
		}
	}
	if (keyboard->isKeyDown(OIS::KC_W)) {
		if (gameObject->getPosition().y > -verticalLimit) {
			gameObject->translate(Vec3(0, -verticalLimit*verticalVel* t, 0));
		}
		if (pivot->getPosition().y < verticalLimit){
			pivot->translate(Vec3(0, verticalLimit*verticalVel * t, 0));
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_S)){

		if (gameObject->getPosition().y < verticalLimit){
			gameObject->translate(Vec3(0, verticalLimit* verticalVel * t, 0));
		}
		if (pivot->getPosition().y > -verticalLimit) {
			pivot->translate(Vec3(0,-verticalLimit*verticalVel* t, 0));
		}

	}

	if (keyboard->isKeyDown(OIS::KC_SPACE)) {
		if (gameObject->getPosition().z < aceleratedCameraOffset) {
			gameObject->translate(Vec3(0, 0, 80 * t));

		}
	}

	else
	{
		if (gameObject->getPosition().z > cameraDefaulOffset) {
			gameObject->translate(Vec3(0, 0, -70 * t));

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