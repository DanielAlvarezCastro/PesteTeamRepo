#include "BasicComponent.h"
#include "GameObject.h"

BasicComponent::BasicComponent(GameObject * gameObject_): gameObject(gameObject_)
{
	gameObject->addReference();
}

BasicComponent::~BasicComponent()
{
	gameObject->release();
}

void BasicComponent::sendObjectMsg(Message * msg_)
{
	gameObject->reciveMsg(msg_);
}

void BasicComponent::sendSceneMsg(Message * msg_)
{
	//gameObject.getScene().revieveMsg(msg_);
}
