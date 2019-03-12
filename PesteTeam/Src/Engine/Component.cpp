#include "Component.h"
#include "GameObject.h"


Component::Component(GameObject * gameObject_): gameObject(gameObject_)
{
	gameObject->addReference();
}

Component::~Component()
{
	gameObject->release();
}

void Component::sendObjectMsg(Message * msg_)
{
	gameObject->reciveMsg(msg_);
}

void Component::sendSceneMsg(Message * msg_)
{
	//gameObject.getScene().revieveMsg(msg_);
}
