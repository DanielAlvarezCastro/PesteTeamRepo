#include "RigidBody.h"
#include"GameObject.h"

RigidBody::RigidBody(GameObject* gameObject_) : BasicComponent(gameObject_)
{
	mass = 0;
	setIniConf();
}

RigidBody::RigidBody(GameObject* gameObject_, btScalar mass_) : BasicComponent(gameObject_), mass(mass_)
{
	setIniConf();
}

RigidBody::RigidBody(GameObject* gameObject_, float density, bool b) : BasicComponent(gameObject_)
{
	//masa = dimension del gameObject * densidad establecida
	mass = gameObject->getScale().x * gameObject->getScale().y * gameObject->getScale().z * density;
	setIniConf();
}

void RigidBody::setIniConf() {
	//forma del collider
	btVector3 auxScale{ gameObject->getScale().x, gameObject->getScale().y, gameObject->getScale().z };
	//de momento solo haremos collider con forma de cubos
	btCollisionShape* shape = new btBoxShape(auxScale);

	//posicion y rotacion
	btTransform startTransform;
	startTransform.setIdentity();
	//rotation del gameobject
	btQuaternion auxDir{ gameObject->getYaw(), gameObject->getPitch(), gameObject->getRoll() };
	startTransform.setRotation(auxDir);
	//inercia inicial por defecto a 0
	btVector3 localInertia{ 0,0,0 };
	
	shape->calculateLocalInertia(mass, localInertia);
	btVector3 auxPos{ gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z };
	startTransform.setOrigin(auxPos);

	//estado inicial del cuerpo
	btDefaultMotionState * motionState = new btDefaultMotionState(startTransform);
	//construimos el rigid body con toda la informacion 
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	rigidBody = new btRigidBody(rbInfo);
	rigidBody->setRestitution(1);
	rigidBody->setUserPointer(gameObject);
	//añadimos el cuerpo al mundo fisico
	Physics::getInstance()->addRigidBodyToWorld(rigidBody, "kk");
}

void RigidBody::setName(const std::string newName) {
	name = newName;
}

RigidBody::~RigidBody()
{
}

void RigidBody::Update()
{
}
