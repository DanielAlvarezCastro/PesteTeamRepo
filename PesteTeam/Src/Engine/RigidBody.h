#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "BasicComponent.h"


#include"Physics.h"

class RigidBody : public BasicComponent
{
protected:
	std::string name;
	btRigidBody* rigidBody;
	btScalar mass;

	void setIniConf();
public:
	///constructora por defecto que deja la masa a 0
	RigidBody(GameObject* gameObject_);
	///constructora con masa por parametro
	RigidBody(GameObject* gameObject_, btScalar mass_);
	///contructora que calcula la masa propia con una densisad
	RigidBody(GameObject* gameObject_, btScalar density_);
	void setName(const std::string newName);

	virtual ~RigidBody();

	virtual void Update();
};

#endif // _RIGIDBODY