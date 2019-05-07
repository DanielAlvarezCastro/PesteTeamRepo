#include "EnemyShoot.h"
#include <ParticleManager.h>


//duplicado de shotBehaviour.cpp
void OnEnemyBulletCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints)
{
	//si tiene rigidbody
	if (other->getRigidBody() != nullptr && one->isActive()) {
		std::cout << "Soy una bala y he chocado" << std::endl;
		one->setActive(false);

		Ogre::Vector3 pos = one->getPosition();
		MainApp::instance()->getParticleManager()->createParticle(pos, "BulletCollision", 1.0f);
	}

	//si es un objeto con comportamiento procesa el choque
	if (other->getBComponents().size() > 0) {
		DownLifeMsg Msg(10, other->getName());
		other->getBComponents()[0]->sendSceneMsg(&Msg);
	}
}

EnemyShoot::EnemyShoot(GameObject* go, enemyType type_, GameObject* target_, std::string bulletMesh): BehaviourComponent(go), type(type_), target(target_), bulletMeshName(bulletMesh)
{
	switch (type)
	{
	case enemyType::groundTurret:
		range = 400;
		ShootCd = 1.6;
		break;
	case enemyType::Flyer:
		ShootCd = 2.4;
		range = 550;
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

void EnemyShoot::Update(float t) {
	auxTemp += t;
	if (auxTemp >= ShootCd) {
		auxTemp = 0;
		shoot();
	}
}

void EnemyShoot::shoot() {
	switch (type)
	{
	case groundTurret:
		getBullet(0);
		break;
	case Flyer:
		//GameObject* b = getBullet();
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
		nBullet->setScale(Vec3(0.5, 0.5, 2.5));

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
	Vec3 aux = gameObject->getGlobalPosition();
	Vec3 dir = gameObject->getDirection();
	switch (type)
	{
	case groundTurret:
		switch (id)
		{
		case 0:
			b->setPosition(aux);
			b->setDirection(dir);
			if (created && i >= 0) {
				bulletComponents[i]->resetValues(aux, dir);
			}
			else {
				BulletBehaviour* bh = new BulletBehaviour(b, aux, gameObject->getDirection());
				MainApp::instance()->getCurrentScene()->addComponent(bh);
				bulletComponents.push_back(bh);
			}
			break;
		case 1:
			break;
		default:
			break;
		}
		break;
	case Flyer:
		break;
	default:
		break;
	}
}
