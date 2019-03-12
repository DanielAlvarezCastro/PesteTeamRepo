#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "BasicComponent.h"

class RigidBody : public BasicComponent
{
public:
	RigidBody(GameObject* gameObject_);
	virtual ~RigidBody();

	virtual void Update();

};

#endif // _RIGIDBODY