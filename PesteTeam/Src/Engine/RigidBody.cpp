#include "RigidBody.h"
#include"GameObject.h"

#include<OgreMeshManager.h>
#include<OgreMesh.h>

RigidBody::RigidBody(GameObject* gameObject_, std::string name_) : BasicComponent(gameObject_), name(name_)
{
	mass = 0;
	setIniConf();
}

RigidBody::RigidBody(GameObject* gameObject_, btScalar mass_, std::string name_) : BasicComponent(gameObject_), mass(mass_), name(name_)
{
	setIniConf();
}

RigidBody::RigidBody(GameObject* gameObject_, std::string name_, float density) : BasicComponent(gameObject_), name(name_)
{
	//masa = dimension del gameObject * densidad establecida
	mass = (gameObject->getScale().x * gameObject->getScale().y * gameObject->getScale().z) * density;
	setIniConf();
	rigidBody->setRestitution(0);
}

RigidBody::~RigidBody()
{
	rigidBody = nullptr;
}

void RigidBody::setIniConf() {
	//forma del collider en funcion de la bounding box del GO
	Vec3 scale = gameObject->getBoundingBox();
	btVector3 auxScale{ scale.x, scale.y, scale.z };
	//de momento solo haremos collider con forma de cubos
	btCollisionShape* shape = new btBoxShape(auxScale);

	//posicion y rotacion
	btTransform startTransform;
	startTransform.setIdentity();

	//rotation del collider para que este a la par que la del GO
	btQuaternion auxDir{ gameObject->getYaw(), gameObject->getPitch(), gameObject->getRoll() };
	startTransform.setRotation(auxDir);
	//inercia inicial por defecto a 0
	btVector3 localInertia{ 0,0,0 };
	
	shape->calculateLocalInertia(mass, localInertia);
	btVector3 auxPos{ gameObject->getPosition().x, gameObject->getPosition().y, gameObject->getPosition().z };
	startTransform.setOrigin(auxPos);

	//estado inicial del cuerpo
	btDefaultMotionState * motionState = new btDefaultMotionState(startTransform);
	//construimos el rigidbody con toda la informacion 
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	rigidBody = new btRigidBody(rbInfo);
	rigidBody->setRestitution(1);
	rigidBody->setUserPointer(gameObject);
	//a�adimos el cuerpo al mundo fisico
	Physics::getInstance()->addRigidBodyToWorld(rigidBody, name);
}

void RigidBody::setName(const std::string newName) {
	name = newName;
}

void RigidBody::setMass(const btScalar nmass_) {
	//necesitamos la inercia local
	btVector3 localInertia = rigidBody->getLocalInertia();
	mass = nmass_;
	//asignamos al rigidbody
	rigidBody->setMassProps(mass, localInertia);
}

void RigidBody::Update(float t)
{
	//posicion del body
	btTransform trans;
	if (rigidBody && rigidBody->getMotionState()) {
		rigidBody->getMotionState()->getWorldTransform(trans);
		//control sobre el gameObject
		btQuaternion rotation = trans.getRotation();
		gameObject->setPosition(Vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
		//sacamos los euler desde el quaternion
		btScalar auxX, auxY, auxZ;
		rotation.getEulerZYX(auxZ, auxY, auxX);
		gameObject->setDirection(Vec3(auxX, auxY, auxZ));
	}
	//comprobamos colision
	auto& manifoldPoints = Physics::getInstance()->getObjectsCollisions()[rigidBody];
	if (! manifoldPoints.empty()) {
		cout << "choco" << endl;
	}

	//actualizamos caja de colision
	Vec3 scale = gameObject->getBoundingBox();
	btVector3 auxScale{ scale.x, scale.y, scale.z };
	delete rigidBody->getCollisionShape();
	btCollisionShape* shape = new btBoxShape(auxScale);
	rigidBody->setCollisionShape(shape);

	btDynamicsWorld* dw = Physics::getInstance()->getDynamicWorld();
	btTransform wt = rigidBody->getWorldTransform();
	btCollisionShape* cs = rigidBody->getCollisionShape();
	btVector3 colour = { 0, 1, 0 };
	
	dw->debugDrawObject(wt, cs, colour);
}
