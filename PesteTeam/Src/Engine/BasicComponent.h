#include <iostream>

#ifndef _BASIC_COMPONENT_H
#define _BASIC_COMPONENT_H

using namespace std;

class GameObject;
struct Message 
{
	Message(string id_) :id(id_){}
	string id;
};

class BasicComponent //Clase abstracta base de los componentes 
{
protected:
	//id de componente?
	GameObject* gameObject;
public:
	BasicComponent(GameObject* gameObject_);
	virtual ~BasicComponent();
	virtual void Update(float t) = 0;

	virtual void sendObjectMsg(Message* msg_);
	virtual void sendSceneMsg(Message* msg_);
	virtual void reciveMsg(Message* msg_) = 0;
};

#endif // _BASI_COMPONENT