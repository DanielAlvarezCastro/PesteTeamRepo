#include "ShotBehaviour.h"
#include "ParticleManager.h"

ShotBehaviour::ShotBehaviour(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	keyboard = MainApp::instance()->getKeyboard();
	scn = MainApp::instance()->getCurrentScene();
}

ShotBehaviour::~ShotBehaviour()
{
	delete keyboard;

	for (int i = 0; i < bullets_.size(); i++) {
		delete bullets_[i];
	}

	// el vector de componentes no debe limpiar memoria aqui
	// lo hara en la escena
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
			cooldown = 3;
			keyDown = false;
		}
	}
}

void ShotBehaviour::getBullets()
{
	int i = 0;
	bool found = false;
	while (i < bullets_.size() && !found)
	{
		if (i < (bullets_.size() - 1) && !bullets_[i]->isActive() && !bullets_[i + 1]->isActive())
			found = true;
		else
			i++;
	}

	if (!found)
	{
		GameObject* bullet = new GameObject();
		string name = "BalaLeft" + to_string(bulletCount);
		bullet->createEntity("SXRVengeanceBullet.mesh", name, scn);
		
		

		GameObject* bullet2 = new GameObject();
		string name2 = "BalaRight" + to_string(bulletCount);
		bullet2->createEntity("SXRVengeanceBullet.mesh", name2, scn);

		bullet->setScale(Vec3(0.5, 0.5, 1.5));
		bullet2->setScale(Vec3(0.5, 0.5, 1.5));

		string rName = "rBalaLeft" + to_string(bulletCount);
		RigidBody* rBullet = new RigidBody(bullet, rName, 10, true);
		bullet->addRigidbody(rBullet);
		scn->addComponent(rBullet);

		string rName2 = "rBalaRight" + to_string(bulletCount);
		RigidBody* rBullet2 = new RigidBody(bullet2, rName2, 10, true);
		bullet2->addRigidbody(rBullet2);
		scn->addComponent(rBullet2);

		Vec3 p1 = scn->getGameObject("Pivot1")->getGlobalPosition();
		Vec3 p2 = scn->getGameObject("Pivot2")->getGlobalPosition();
		BulletBehaviour* bb = new BulletBehaviour(bullet, p1, gameObject->getDirection());
		scn->addComponent(bb);
		bComponents_.push_back(bb);
		BulletBehaviour* bb2 = new BulletBehaviour(bullet2, p2, gameObject->getDirection());
		scn->addComponent(bb2);
		bComponents_.push_back(bb2);

		scn->addGameObject(bullet);
		scn->addGameObject(bullet2);

		bullets_.push_back(bullet);
		bullets_.push_back(bullet2);

		bulletCount++;
		
		MainApp::instance()->getParticleManager()->createParticle(p2, scn, "Cross", 1.0);

		std::pair<GameObject*, GameObject*> blls(bullet, bullet2);
		UpdateValues(-1, blls);
	}

	else
	{
		Vec3 p2 = scn->getGameObject("Pivot2")->getGlobalPosition();
		MainApp::instance()->getParticleManager()->createParticle(p2, scn, "Cross", 1.0);
		std::pair<GameObject*, GameObject*> blls(bullets_[i], bullets_[i + 1]);
		UpdateValues(i, blls);
	}
}

void ShotBehaviour::Shoot()
{
	getBullets();
}

void ShotBehaviour::UpdateValues(int i, std::pair<GameObject*, GameObject*> blls)
{
	Ogre::Vector3 auxDir = gameObject->getDirection();
	auxDir.normalise();

	Ogre::Vector3 centPos = gameObject->getPosition();
	Vec3 auxPosLeft = scn->getGameObject("Pivot1")->getGlobalPosition();
	Vec3 auxPosRight = scn->getGameObject("Pivot2")->getGlobalPosition();

	blls.first->setPosition(auxPosLeft);
	blls.first->setDirection(gameObject->getDirection());
	blls.first->setActive(true);
	if (i >= 0)
		bComponents_[i]->resetValues(auxPosLeft, gameObject->getDirection());

	blls.second->setPosition(auxPosRight);
	blls.second->setDirection(gameObject->getDirection());
	blls.second->setActive(true);
	if (i >= 0)
		bComponents_[i + 1]->resetValues(auxPosRight, gameObject->getDirection());
}