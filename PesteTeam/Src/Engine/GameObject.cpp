#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::release()
{
	references--;
	if (references == 0) 
	{
		if(rigidBody) delete rigidBody;
		for (auto &bComponent : behaviourComponents)
		{
			delete bComponent;
		}
		//^Delete dynamic objects^
		delete this;
	}
}

void GameObject::reciveMsg(Message * msg)
{
	if (rigidBody) rigidBody->reciveMsg(msg);
	for (auto &bComponent: behaviourComponents)
	{
		bComponent->reciveMsg(msg);
	}
}

void GameObject::createEntity(std::string mesh) 
{
	ogreEntity = MainApp::instance()->getSceneMgr()->createEntity(mesh);

	ogreNode = MainApp::instance()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);
	ogreNode->setScale(0.1, 0.1, 0.1);
}