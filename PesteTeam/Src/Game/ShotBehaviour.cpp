#include "ShotBehaviour.h"
#include "ParticleManager.h"

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
		DownLifeMsg Msg(10, other->getName());
		other->getBComponents()[0]->sendSceneMsg(&Msg);
	}
}

ShotBehaviour::ShotBehaviour(GameObject* gameObject, std::string shipName) : BehaviourComponent(gameObject), shipName_(shipName)
{
	keyboard = MainApp::instance()->getKeyboard();
	scn = MainApp::instance()->getCurrentScene();
	bulletMeshName = shipName + "Bullet.mesh";
	bulletMaterialName = shipName + "Bullet.material";
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
		shoot();
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

void ShotBehaviour::reciveMsg(Message * msg)
{
	if (msg->id == "BULLET_COLLISION") {
		BulletCollisionMsg* dlm = static_cast<BulletCollisionMsg*>(msg);
		MainApp::instance()->getParticleManager()->createParticle(dlm->pos, "BulletCollision", 1.0f, bulletMaterialName);
	}
}

void ShotBehaviour::getBullet(int id)
{
	int i = 0;
	while (i < bullets_.size() && bullets_[i]->isActive())
		i++;

	if (i >= bullets_.size()) {
		GameObject* nBullet = new GameObject();
		string name = gameObject->getName() + "PlayerBullet" + to_string(i);
		nBullet->createEntity(bulletMeshName, name, MainApp::instance()->getCurrentScene());
		nBullet->setScale(Vec3(0.5, 0.5, 2.5));

		string rBulletName = "PlayerRb" + name;
		RigidBody* rBullet = new RigidBody(nBullet, 1, rBulletName, true);
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
		pos = scn->getGameObject("Pivot1")->getGlobalPosition();
		b->setPosition(pos);
		break;
	case 1:
		pos = scn->getGameObject("Pivot2")->getGlobalPosition();
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
		BulletBehaviour* bh = new BulletBehaviour(b, pos, dir);
		scn->addComponent(bh);
		bComponents_.push_back(bh);
	}
}