#include "ShipController.h"
#include <GameObject.h>
#include "GameSceneManager.h"
#include "ParticleManager.h"


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
			euler.rotate(Radian(0), Radian(0),Radian(rollVel)*t);
			gameObject->setOrientation(euler);
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		if (euler.mRoll > Radian(-1.0)) {
			euler.rotate(Radian(0), Radian(0), -Radian(rollVel)*t);
			gameObject->setOrientation(euler);
		}
	}
	else 
	{
		if (euler.mRoll > Radian(0.02) && !isRolling)
		{
			euler.rotate(Radian(0), Radian(0), -Radian(rollBackVel)*t);
			gameObject->setOrientation(euler);
		}
		else if (euler.mRoll < Radian(-0.02) && !isRolling)
		{
			euler.rotate(Radian(0), Radian(0), Radian(rollBackVel)*t);
			gameObject->setOrientation(euler);
		}
	}

	if (keyboard->isKeyDown(OIS::KC_E)) {
		if (!isRolling) {
			isRolling = true;
			rollRight = true;
			iniOrientation = euler.mRoll;
		}
	}
	if (keyboard->isKeyDown(OIS::KC_Q)) {
		if (!isRolling) {
			isRolling = true;
			rollLeft = true;
			iniOrientation = euler.mRoll;
		}
	}
	if (isRolling) {
		if (rollRight && euler.mRoll > (-Radian(2 * pi) + iniOrientation) + Radian(2 * pi)*t) {
			euler.rotate(Radian(0), Radian(0), -Radian(barrelVel)*t);
			gameObject->setOrientation(euler);
		}
		else if (rollLeft && euler.mRoll < (Radian(2 * pi) + iniOrientation)- Radian((2 * pi)*t)) {
			euler.rotate(Radian(0), Radian(0), Radian(barrelVel)*t);
			gameObject->setOrientation(euler);
		}
		else {
			cout << euler.mRoll << endl;
			cout << iniOrientation << endl;
			isRolling = false;
			rollLeft = false;
			rollRight = false;
			euler.mRoll = iniOrientation;
		}
	}
}
