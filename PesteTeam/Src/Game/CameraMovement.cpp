#include "CameraMovement.h"
#include <GameObject.h>
CameraMovement::CameraMovement(GameObject * camera_, GameObject * target_) :BehaviourComponent(camera_), camera(camera_), target(target_)
{
}

CameraMovement::~CameraMovement()
{
}

void CameraMovement::Update()
{
	Vec3 targetPos = target->getPosition();
	targetPos.z -= 100;
	camera->lookAt(target->getPosition());
}
