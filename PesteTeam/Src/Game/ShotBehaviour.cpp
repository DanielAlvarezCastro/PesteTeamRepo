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
			cooldown = 50;
			keyDown = false;
		}
	}
}

void ShotBehaviour::Shoot()
{
	GameObject* bullet = new GameObject();
	string name = "Bala" + to_string(bulletCount);
	bullet->createEntity("SXRVengeanceBullet.mesh", name, scn);
	
	bullet->setScale(Vec3(0.4, 0.4, 1.5));
	bullet->setPosition(gameObject->getPosition());
	bullet->setDirection(gameObject->getDirection());

	string rName = "rBala" + to_string(bulletCount);
	RigidBody* rBullet = new RigidBody(bullet, rName, 10, true);
	bullet->addRigidbody(rBullet);
	scn->addComponent(rBullet);

	BulletBehaviour* bb = new BulletBehaviour(bullet);
	scn->addComponent(bb);

	scn->addGameObject(bullet);
	bulletCount++;
}