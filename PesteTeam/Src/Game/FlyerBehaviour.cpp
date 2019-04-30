#include "FlyerBehaviour.h"
#include <GameObject.h>

FlyerBehaviour::FlyerBehaviour(GameObject* go, GameObject* target_, float radius_, float height_): BehaviourComponent(go), target(target_),
								radius(radius_), height(height_){
	//centre = go->getGlobalPosition();
	cx = go->getGlobalPosition().x;
	cy = go->getGlobalPosition().y;
	cz = go->getGlobalPosition().z;
}

FlyerBehaviour::~FlyerBehaviour() {
	target = nullptr;
}

void FlyerBehaviour::Update(float t) {
	timePassed += t;
	if (timePassed >= 360)
		timePassed -= 360;
	//la coordenada y es la altura
	Vec3 newPos = Vec3(cx + (Math::Sin(timePassed) * radius)/2 ,cy + ( Math::Sin(timePassed*2.5)*height), cz + (Math::Cos(timePassed)*radius)/2);

	gameObject->setPosition(newPos);
	//Hace un lookAt pero sin hacer yaw (rotar en el eje y)
	Ogre::Vector3 dir = gameObject->getPosition() - target->getPosition();
	dir.normalise();
	Ogre::Vector3 right(dir.z, 0, -dir.x);
	right.normalise();
	Ogre::Vector3 up = dir.crossProduct(right);
	gameObject->setOrientation(Ogre::Quaternion(right, up, dir));
}

void FlyerBehaviour::reciveMsg(Message* msg) {

}