#ifndef _BEHAVIOURCOMPONENT_H
#define _BEHAVIOURCOMPONENT_H

#include "Component.h"
class BehaviourComponent :
	public Component
{
public:
	BehaviourComponent(GameObject* gameObject_);
	virtual ~BehaviourComponent();

	virtual void Update();

};

#endif // _BEHAVIOURCOMPONENT

