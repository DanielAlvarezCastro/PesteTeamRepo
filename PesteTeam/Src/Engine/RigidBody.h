#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "BasicComponent.h"
#include"Physics.h"
#include<Ogre.h>

class RigidBody : public BasicComponent
{
protected:
	std::string name;
	btRigidBody* rigidBody;
	btScalar mass;

	//para ver las lineas del collider en pantalla
	Ogre::MeshPtr debugCollider;		

	void setIniConf();
public:
	///constructora por defecto que deja la masa a 0 (objeto inamovible)
	RigidBody(GameObject* gameObject_, std::string name_);
	///constructora con masa por parametro
	RigidBody(GameObject* gameObject_, btScalar mass_, std::string name_);
	///contructora que calcula la masa propia con una densisad
	RigidBody(GameObject* gameObject_, std::string name_, float density_);
	void setName(const std::string newName);
	void setMass(const btScalar nmass_);

	void setGravity(btScalar x, btScalar y, btScalar z) { if (rigidBody != NULL) rigidBody->setGravity(btVector3(x,y,z)); };

	virtual void reciveMsg(Message* msg_) {};
	virtual ~RigidBody();
	virtual void Update(float t);
};

#endif // _RIGIDBODY