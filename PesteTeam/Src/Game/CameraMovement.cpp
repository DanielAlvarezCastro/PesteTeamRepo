#include "CameraMovement.h"
#include <GameObject.h>
CameraMovement::CameraMovement(GameObject * camera_, GameObject * target_) :BehaviourComponent(camera_), camera(camera_), target(target_)
{
}

CameraMovement::~CameraMovement()
{
}


void CameraMovement::Update(float t)
{
	/*Vec3 targetPos = target->getPosition();
	targetPos.z -= 100;
	camera->lookAt(target->getPosition());*/

	Ogre::Vector3 moveTo = target->getPosition() - target->getDirection() * 1000.0f + Ogre::Vector3(0,1,0) * 1.0f;
	float bias = 0.90f;
	Ogre::Vector3 pos = target->getPosition() * bias + moveTo * (1.0f - bias);
	camera->setPosition(pos);
	camera->lookAt(target->getPosition() + target->getDirection() * 10.0f);
}
