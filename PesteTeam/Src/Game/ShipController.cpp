#include "ShipController.h"
#include <GameObject.h>
#include "GameSceneManager.h"


ShipController::ShipController(GameObject* gameObject) :BehaviourComponent(gameObject)
{
	keyboard = MainApp::instance()->getKeyboard();
	gameObject->setOrientation(euler);
}


ShipController::~ShipController()
{
	delete keyboard;
}

void ShipController::Update(float t)
{
	if (keyboard->isKeyDown(OIS::KC_A)) {
		if (euler.mRoll < Radian(1.0)) {
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian((pi / 2)*t));
			gameObject->setOrientation(euler);
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		if (euler.mRoll > Radian(-1.0)) {
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian(-(pi / 2)*t));
			gameObject->setOrientation(euler);
		}
	}
	else 
	{
		if (euler.mRoll > Radian(0.02) && !isRolling)
		{
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian(-(pi / 4)*t));
			gameObject->setOrientation(euler);
		}
		else if (euler.mRoll < Radian(-0.02) && !isRolling)
		{
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian((pi / 4)*t));
			gameObject->setOrientation(euler);
		}
	}

	if (keyboard->isKeyDown(OIS::KC_E) && canRoll) {
		if (euler.mRoll < Radian(1.0) && euler.mRoll > Radian(-1.0)  && !isRolling) {
			isRolling = true;
			rollRight = true;
			iniOrientation = euler.mRoll;
		}
	}
	if (keyboard->isKeyDown(OIS::KC_Q) && canRoll) {
		if (euler.mRoll > Radian(-1.0) && euler.mRoll > Radian(-1.0) && !isRolling) {
			isRolling = true;
			rollLeft = true;
			iniOrientation = euler.mRoll;
		}
	}
	if (isRolling) {
		if (rollRight && euler.mRoll > (Radian(-2.0 * pi) + iniOrientation)) {
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian((-5 * pi)*t));
			gameObject->setOrientation(euler);
		}
		else if (rollLeft && euler.mRoll < (Radian(2.0 * pi) + iniOrientation)) {
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian((5 * pi)*t));
			gameObject->setOrientation(euler);
		}
		else {
			isRolling = false;
			canRoll = true;
			rollLeft = false;
			rollRight = false;
			euler.mRoll = iniOrientation;
		}
	}
}
