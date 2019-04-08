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
		if (euler.mRoll > Radian(0.05)) 
		{
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian(-(pi / 4)*t));
			gameObject->setOrientation(euler);
		}
		else if (euler.mRoll < Radian(-0.02)) 
		{
			euler.rotate(Ogre::Radian(0), Ogre::Radian(0), Ogre::Radian((pi / 4)*t));
			gameObject->setOrientation(euler);
		}

	}

}
