#include "ShotBehaviour.h"

ShotBehaviour::ShotBehaviour(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	keyboard = MainApp::instance()->getKeyboard();
	scn = MainApp::instance()->getCurrentScene();
}

ShotBehaviour::~ShotBehaviour()
{
	delete keyboard;
}

void ShotBehaviour::Update(float t)
{
	if (keyboard->isKeyDown(OIS::KC_L) && !keyDown) 
	{
		Shoot();
		keyDown = true;
	}

	if (keyDown) 
	{
		if (cooldown > 0)
			cooldown--;
		else
		{
			cooldown = 10;
			keyDown = false;
		}
	}
}

void ShotBehaviour::Shoot()
{
	GameObject* bullet = new GameObject();
	string name = "BalaLeft" + to_string(bulletCount);
	bullet->createEntity("SXRVengeanceBullet.mesh", name, scn);

	GameObject* bullet2 = new GameObject();
	string name2 = "BalaRight" + to_string(bulletCount);
	bullet2->createEntity("SXRVengeanceBullet.mesh", name2, scn);
	
	Ogre::Vector3 auxDir = gameObject->getDirection();
	auxDir.normalise();

	Ogre::Vector3 centPos = gameObject->getPosition();
	Vec3 auxPosLeft = scn->getGameObject("Pivot1")->getGlobalPosition();
	Vec3 auxPosRight = scn->getGameObject("Pivot2")->getGlobalPosition();

	bullet->setScale(Vec3(0.4, 0.4, 1.5));
	bullet->setPosition(auxPosLeft);
	bullet->setDirection(gameObject->getDirection());

	bullet2->setScale(Vec3(0.4, 0.4, 1.5));
	bullet2->setPosition(auxPosRight);
	bullet2->setDirection(gameObject->getDirection());

	string rName = "rBalaLeft" + to_string(bulletCount);
	RigidBody* rBullet = new RigidBody(bullet, rName, 10, true);
	bullet->addRigidbody(rBullet);
	scn->addComponent(rBullet);

	string rName2 = "rBalaRight" + to_string(bulletCount);
	RigidBody* rBullet2 = new RigidBody(bullet2, rName2, 10, true);
	bullet2->addRigidbody(rBullet2);
	scn->addComponent(rBullet2);

	BulletBehaviour* bb = new BulletBehaviour(bullet);
	scn->addComponent(bb);

	BulletBehaviour* bb2 = new BulletBehaviour(bullet2);
	scn->addComponent(bb2);

	scn->addGameObject(bullet);
	scn->addGameObject(bullet2);

	bulletCount++;
}