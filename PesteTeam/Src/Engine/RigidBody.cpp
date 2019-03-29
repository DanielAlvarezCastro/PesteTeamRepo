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
}

void RigidBody::setIniConf() {
	//guardamos la rotacion original del GO
	Vec3 ogRotation{ gameObject->getDirection() };
	//rotamos el GO para que se alinee con cualquiera de los ejes, con el fin de que su bounding box sea mínima
	gameObject->setDirection(Vec3{ 0,0,0 });
	//forma del collider en funcion de la bounding box del GO
	Vec3 scale = gameObject->getBoundingBox();
	btVector3 auxScale{ scale.x, scale.y, scale.z };
	//de momento solo haremos collider con forma de cubos
	btCollisionShape* shape = new btBoxShape(auxScale);

	//posicion y rotacion
	btTransform startTransform;
	startTransform.setIdentity();

	//devolvemos el GO a su rotacion original antes de girar tambien el collider creado
	gameObject->setDirection(ogRotation);
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

RigidBody::~RigidBody()
{
	rigidBody = nullptr;
}

void RigidBody::Update(float t)
{
	//puntero auxiliar para ahorrar llamadas
	btDynamicsWorld* auxWolrd = Physics::getInstance()->getDynamicWorld();
	if (auxWolrd != NULL) {
		//actualizamos el tiempo
		auxWolrd->stepSimulation(t);
		//accedemos a cada uno de los objetos
		for (int i = 0; i < Physics::getInstance()->getNumberOfBodies(); i++) {
			btCollisionObject* obj = auxWolrd->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			//posicion del body
			btTransform t;
			if (body && body->getMotionState()) {
				body->getMotionState()->getWorldTransform(t);
			}
			//control sobre el gameObject
			btQuaternion rotation = t.getRotation();
			gameObject->setPosition(Vec3(t.getOrigin().x(), t.getOrigin().y(), t.getOrigin().z()));
			//sacamos los euler desde el quaternion
			btScalar auxX, auxY, auxZ;
			rotation.getEulerZYX(auxZ, auxY, auxX);
			gameObject->setDirection(Vec3(auxX, auxY, auxZ));
		}
	}
}
