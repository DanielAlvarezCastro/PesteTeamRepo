#include "PlayerController.h"
#include <GameObject.h>
#include "GameSceneManager.h"


PlayerController::PlayerController(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	keyboard = MainApp::instance()->getKeyboard();
	gameObject->setOrientation(euler);

}


PlayerController::~PlayerController()
{
	delete keyboard;
}

void PlayerController::Update(float t)
{
	if (keyboard->isKeyDown(OIS::KC_A)) {
		euler.rotate(Ogre::Radian(rotationVel*t), Ogre::Radian(0), Ogre::Radian(0));
		gameObject->setOrientation(euler);
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		euler.rotate(Ogre::Radian(-rotationVel *t), Ogre::Radian(0), Ogre::Radian(0));
		gameObject->setOrientation(euler);
	}
	if (keyboard->isKeyDown(OIS::KC_W)) {
		if(Math::Abs(euler.mPitch) < Radian(pitchLimit)){
			euler.rotate(Ogre::Radian(0), Ogre::Radian(-rotationVel *t), Ogre::Radian(0));
			gameObject->setOrientation(euler);
			moved = true;
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_S)) {
		if (Math::Abs(euler.mPitch) < Radian(pitchLimit)){
			euler.rotate(Ogre::Radian(0), Ogre::Radian(rotationVel*t), Ogre::Radian(0));
			gameObject->setOrientation(euler);
			moved = true;
		}
	}
	if(!keyboard->isKeyDown(OIS::KC_W) && !keyboard->isKeyDown(OIS::KC_S))//Reduce el pitch a 0
	{
		if (Math::Abs(euler.mPitch) > Radian((0.01)))
		{
			if (moved) {
				lastT = 0; moved = false; 
				e.setRoll(Radian(0)); e.setPitch(Radian(0)); e.setYaw(Radian(0));
				e.rotate(euler.mYaw, Ogre::Radian(0), Ogre::Radian(0));
			}
			if (lastT < 1 && !moved)
			{				
				lastT += t / 8;
				euler.normalise();
				Euler angle = euler.rotationTo(Ogre::Vector3(euler.forward().x, 0, euler.forward().z));
				euler = euler + angle * lastT;
				gameObject->setOrientation(euler);
			}
		}
	}
	if (keyboard->isKeyDown(OIS::KC_SPACE)) {
		gameObject->translate(euler.forward() * fastSpeed * t);
	}
	else {
		gameObject->translate(euler.forward() * normalSpeed* t);
	}
	if (keyboard->isKeyDown(OIS::KC_P)) {
		GameSceneManager::instance()->CloseScene();

		SoundManager::instance()->GetEngine()->stopAllSounds();
		SoundManager::instance()->PlaySound2D("SynthSong3.mp3", true, false);
	}
}