#include "ShotBehaviour.h"
#include "ParticleManager.h"
#include <random>
#include <string>
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>



void OnBulletCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints) 
{	
	//si tiene rigidbody
	if (other->getRigidBody() != nullptr && other->isActive() && one->isActive()) { 
		//std::cout << "Soy una bala y he chocado" << std::endl;
		one->setActive(false);
		
		Ogre::Vector3 pos = one->getPosition();
		BulletCollisionMsg Msg(pos);
		if(other->getBComponents().size() > 0)
			other->getBComponents()[0]->sendSceneMsg(&Msg);
		
	}

	//si es un objeto con comportamiento procesa el choque
	if (other->getBComponents().size() > 0) { 
		BulletCollideEntity Msg(other->getPosition(), other->getName());
		other->getBComponents()[0]->sendSceneMsg(&Msg);
	}
}

ShotBehaviour::ShotBehaviour(GameObject* gameObject, std::string shipName, int _damage, int maxOverload_, float shotCD, float ovRechargeMultiplier_) 
	: BehaviourComponent(gameObject),shipName_(shipName), damage(_damage), cooldown(shotCD), maxOverload(maxOverload_), ovRechargeMultiplier(ovRechargeMultiplier_)
{
	keyboard = MainApp::instance()->getKeyboard();
	scn = MainApp::instance()->getCurrentScene();
	bulletMeshName = shipName + "Bullet.mesh";
	bulletParticleCollisionName = shipName + "BulletCollision";
}

ShotBehaviour::~ShotBehaviour()
{
}

void ShotBehaviour::Update(float t)
{
	if (overloaded) {
		cOverload -= t*ovRechargeMultiplier;
		//cout << cOverload << endl;
		if (cOverload <= 0) {
			cout << "Ya puedo volver a disparar" << endl;
			cOverload = 0;
			overloaded = false;
		}
	}
	if (!overloaded && keyboard->isKeyDown(OIS::KC_L) && !keyDown)
	{
		ISound* aux = SoundManager::instance()->PlaySound2D("ShootPlayer.wav");
		aux->setVolume(0.9);
		shoot();
		keyDown = true;
		//aqui llegara cada vez que dispare, que ocurre cada cooldown de tiempo
		cOverload+=cooldown;
		//cout << cOverload << endl;
		if (cOverload >= maxOverload) {
			cout << "Sobrecalentado, volveré a poder disparar en: " << maxOverload << endl;
			overloaded = true;
		}
	}
	if (keyDown) 
	{
		if (currentCd > 0)
			currentCd-= t;
		else
		{
			currentCd = cooldown;
			keyDown = false;
		}
	}
	if (!keyboard->isKeyDown(OIS::KC_L) && !overloaded) {
		cOverload -= t;
		if (cOverload < 0)
			cOverload = 0;
		//cout << cOverload << endl;
	}
}

void ShotBehaviour::reciveMsg(Message * msg)
{
	if (msg->id == "BULLET_COLLISION") {
		BulletCollisionMsg* dlm = static_cast<BulletCollisionMsg*>(msg);
		ISound* aux = SoundManager::instance()->PlaySound2D("HurtEnemy.wav");
		aux->setVolume(0.8);
		MainApp::instance()->getParticleManager()->createParticle(dlm->pos, bulletParticleCollisionName, 1.0f);
	}
	else if (msg->id == "BULLET_COLLIDE_ENTITY") {
		BulletCollideEntity* bce = static_cast<BulletCollideEntity*>(msg);
		if (bce->name == "PointerPlayer") {
			
			DownLifeMsg Msg(damage, bce->name);
			sendSceneMsg(&Msg);
		}
		
	}
}

void ShotBehaviour::getBullet(int id)
{
	int i = 0;
	while (i < bullets_.size() && bullets_[i]->isActive())
		i++;

	if (i >= bullets_.size()) {
		GameObject* nBullet = new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) GameObject();
		string name = gameObject->getName() + "PlayerBullet" + to_string(i);
		nBullet->createEntity(bulletMeshName, name, MainApp::instance()->getCurrentScene());
		nBullet->setScale(Vec3(0.5, 0.5, 2.5));

		string rBulletName = "PlayerRb" + name;
		RigidBody* rBullet = new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) RigidBody(nBullet, 1, rBulletName, true);
		rBullet->setCollisionCallback(OnBulletCollision);
		scn->addComponent(rBullet);
		situateBullet(nBullet, id);

		bullets_.push_back(nBullet);
		scn->addGameObject(nBullet);
	}

	else
	{
		bullets_[i]->setActive(true);
		situateBullet(bullets_[i], id, true, i);
	}
}

void ShotBehaviour::shoot()
{
	getBullet(0);
	getBullet(1);
}

void ShotBehaviour::situateBullet(GameObject* b, int id, bool created, int i) 
{
	Vec3 pos;

	switch (id) {
	case 0:
		pos = scn->getGameObject(gameObject->getName() + "LeftGunPivot")->getGlobalPosition();
		b->setPosition(pos);
		break;
	case 1:
		pos = scn->getGameObject(gameObject->getName() + "RightGunPivot")->getGlobalPosition();
		b->setPosition(pos);
		break;
	default:
		break;
	}

	Vec3 dir = gameObject->getDirection();
	b->setDirection(dir);

	if (created && i >= 0) 
		bComponents_[i]->resetValues(pos, dir);
	else {
		BulletBehaviour* bh = new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) BulletBehaviour(b, pos, dir);
		scn->addComponent(bh);
		bComponents_.push_back(bh);
	}
}