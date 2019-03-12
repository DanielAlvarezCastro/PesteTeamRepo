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
