#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"
class FlyerBehaviour :
	public BehaviourComponent
{
private:
	GameObject* target;
	//Vec3 centre;
	float cx,
		cy,
		cz;
	float radius,
		height;
	//controlador para calcular funciones senoidales
	float timePassed = 0;
public:
	FlyerBehaviour(GameObject* gameObject, GameObject* target, float radius_, float height_);
	virtual ~FlyerBehaviour();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};
