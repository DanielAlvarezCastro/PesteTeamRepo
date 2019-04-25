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
	Vec3 newPos = Vec3(cx + (Math::Sin(timePassed) * radius),cy + ( Math::Sin(timePassed*4)*height),cz + (Math::Cos(timePassed)*radius));

	gameObject->setPosition(newPos);
}

void FlyerBehaviour::reciveMsg(Message* msg) {

}