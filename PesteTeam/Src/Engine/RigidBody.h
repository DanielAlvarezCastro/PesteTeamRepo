#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "Component.h"
class RigidBody :
	public Component
{
public:
	RigidBody(GameObject* gameObject_);
	virtual ~RigidBody();

	virtual void Update();

};

#endif // _RIGIDBODY

