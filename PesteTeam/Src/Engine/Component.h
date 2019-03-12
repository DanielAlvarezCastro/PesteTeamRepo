#include <iostream>

#ifndef _COMPONENT_H
#define _COMPONENT_H

using namespace std;

class GameObject;
struct Message 
{
	Message(string id_) :id(id_){}
	string id;
};

class Component //Clase abstracta base de los componentes 
{
protected:
	//id de componente?
	GameObject* gameObject;
public:
	Component(GameObject* gameObject_);
	virtual ~Component();
	virtual void Update() = 0;

	virtual void sendObjectMsg(Message* msg_);
	virtual void sendSceneMsg(Message* msg_);
	virtual void reciveMsg(Message* msg_) = 0;
};

#endif // _COMPONENT