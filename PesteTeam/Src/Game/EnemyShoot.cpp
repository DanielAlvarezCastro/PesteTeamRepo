#include "EnemyShoot.h"
#include <ParticleManager.h>


//duplicado de shotBehaviour.cpp
void OnEnemyBulletCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints)
{
	//si tiene rigidbody
	if (other->getRigidBody() != nullptr  && other->getRigidBody()->isActive() && other->isActive() && one->isActive()) {
		//std::cout << "Soy una bala enemiga y he chocado" << std::endl;
		one->setActive(false);

		Ogre::Vector3 pos = one->getPosition();
		MainApp::instance()->getParticleManager()->createParticle(pos, "EnemyBulletCollision", 1.0f);
	}

	//si es un objeto con comportamiento y con rigidbody activo procesa el choque
	if (other->getBComponents().size() > 0 && other->getRigidBody()!=nullptr && other->getRigidBody()->isActive()) {
		BulletCollideEntity Msg(other->getPosition(), other->getName());
		other->getBComponents()[0]->sendSceneMsg(&Msg);
	}
}

EnemyShoot::EnemyShoot(GameObject* go, enemyType type_, GameObject* target_, int _damage, int _range, std::string bulletMesh): BehaviourComponent(go), type(type_), target(target_), bulletMeshName(bulletMesh), damage(_damage), range(_range)
{
	switch (type)
	{
	case enemyType::groundTurret:
		ShootCd = 1.6;
		break;
	case enemyType::Flyer:
		ShootCd = 2.4;
		break;
	default:
		break;
	}
}

EnemyShoot::~EnemyShoot()
{
	target = nullptr;
	while (!bullets.empty())
		bullets.pop_back();
}

bool EnemyShoot::inRange() {
	//gameObject->getGlobalPosition().getVector().
	float distance =gameObject->getGlobalPosition().getVector().distance(target->getGlobalPosition().getVector());
	return range >= distance;
}

void EnemyShoot::Update(float t) {
	auxTemp += t;
	if (auxTemp >= ShootCd) {
		if (inRange()) {
			auxTemp = 0;
			shoot();
			ISound* aux = SoundManager::instance()->PlaySound2D("ShootEnemy.wav");
			aux->setVolume(0.7);
		}
	}
}

void EnemyShoot::reciveMsg(Message * msg)
{
	if (msg->id == "BULLET_COLLIDE_ENTITY") {
		BulletCollideEntity* bce = static_cast<BulletCollideEntity*>(msg);
		if (bce->name != "PointerPlayer") {
			DownLifeMsg Msg(damage, bce->name);
			sendSceneMsg(&Msg);
		}
	}
}

void EnemyShoot::shoot() {
	switch (type)
	{
	case groundTurret:
		getBullet(0);
		getBullet(1);
		break;
	case Flyer:
		getBullet();
		break;
	default:
		break;
	}
}

void EnemyShoot::getBullet(int id) {
	int i = 0;
	while (i < bullets.size() && bullets[i]->isActive())
		i++;

	if (i >= bullets.size()) {
		GameObject* nBullet = new GameObject();
		string name = gameObject->getName() + "Bullet" + to_string(i);
		nBullet->createEntity(bulletMeshName, name, MainApp::instance()->getCurrentScene());
		nBullet->setScale(Vec3(1, 1, 3.5));

		string rBulletName = "Rb" + name;
		RigidBody* rBullet = new RigidBody(nBullet, 1, rBulletName, true);
		rBullet->setCollisionCallback(OnEnemyBulletCollision);
		MainApp::instance()->getCurrentScene()->addComponent(rBullet);
		situateBullet(nBullet, id);

		bullets.push_back(nBullet);
		MainApp::instance()->getCurrentScene()->addGameObject(nBullet);
	}
	else { 
		bullets[i]->setActive(true);
		situateBullet(bullets[i], id, true, i); 
	}
}

void EnemyShoot::situateBullet(GameObject*b, int id, bool created, int i) {
	Vec3 dir = gameObject->getDirection();
	b->setDirection(dir);
	int vel = 0;
	switch (type)
	{
	case groundTurret:
		switch (id)
		{
		case 0:
			b->setPosition(MainApp::instance()->getCurrentScene()->getGameObject(gameObject->getName() + "PivotT1")->getGlobalPosition());
			break;
		case 1:
			b->setPosition(MainApp::instance()->getCurrentScene()->getGameObject(gameObject->getName() + "PivotT2")->getGlobalPosition());
			break;
		default:
			break;
		}
		vel = 15;
		break;
	case Flyer:
		b->setPosition(MainApp::instance()->getCurrentScene()->getGameObject(gameObject->getName() + "PivotF1")->getGlobalPosition());
		vel = 30;
		break;
	default:
		break;
	}
	if (created && i >= 0)
		bulletComponents[i]->resetValues(b->getGlobalPosition(), dir);
	else {
		BulletBehaviour* bh = new BulletBehaviour(b, b->getGlobalPosition(), gameObject->getDirection());
		bh->setVel(vel);
		MainApp::instance()->getCurrentScene()->addComponent(bh);
		bulletComponents.push_back(bh);
	}
}
