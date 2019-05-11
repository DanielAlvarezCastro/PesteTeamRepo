#include "ShipController.h"
#include <GameObject.h>
#include "GameSceneManager.h"
#include "ParticleManager.h"
#include "Messages.h"

ShipController::ShipController(GameObject* gameObject, int _health) :BehaviourComponent(gameObject), health(_health)
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

	Vec3 pos = gameObject->getGlobalPosition();
	//Si se sale de la zona de combate manda un mensaje
	if ((pos.x > 1400 || pos.x < -1400 || pos.z> 1400 || pos.z<-1400 || pos.y>1500) && !warningZone) {
		EnterWarningZone msg;
		sendSceneMsg(&msg);
		warningZone = true;
	}
	else if (!(pos.x > 1400 || pos.x < -1400 || pos.z> 1400 || pos.z < -1400 || pos.y>1400) && warningZone) {
		ExitWarningZone msg;
		sendSceneMsg(&msg);
		warningZone = false;
	}
	if ((pos.x > 1900 || pos.x < -1900 || pos.z> 1900 || pos.z < -1900 || pos.y>1900 || pos.y<=0)) {
		GameOverMsg msg;
		sendSceneMsg(&msg);
		ExitWarningZone msg2;
		sendSceneMsg(&msg2);
	}
}

void ShipController::reciveMsg(Message * msg)
{
	if (msg->id == "QUITA_VIDA")
	{
		DownLifeMsg* dlm = static_cast<DownLifeMsg*>(msg);
		if (dlm->name == "PointerPlayer") {
			cout << "Daño: " << health << endl;
			health -= dlm->power;
			ISound* aux = SoundManager::instance()->PlaySound2D("HurtPlayer.wav");
			UpdateHealthBarMsg uhb(health);
			sendSceneMsg(&uhb);
			if (health <= 0) {
				GameOverMsg msg;
				sendSceneMsg(&msg);
				ISound* aux = SoundManager::instance()->PlaySound2D("DeathPlayer.wav");
			}
		}
	}
}
