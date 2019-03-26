#include"Physics.h"

Physics::Physics() {
	initPhysics();
}

Physics::~Physics() {
	//limpiamos punteros
	collisionConfiguration = nullptr;
	dispatcher = nullptr;
	overlappingPairCache = nullptr;
	solver = nullptr;
	dynamicsWorld = nullptr;
	//vaciamos vector de collisionShapes
	for (btCollisionShape* b : collisionShapes) {
		delete b;
	}
	//limpiamos el map
	physicsAccessors.clear();
}

void Physics::initPhysics() {
	//inicializamos los elementos necesarios
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	//creamos el mundo fisico
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}

void Physics::addRigidBodyToWorld(btRigidBody* bt,const std::string & btname) {
	//conseguimos la forma
	btCollisionShape* auxShape = bt->getCollisionShape();
	//y la añadimos al mundo
	collisionShapes.push_back(auxShape);
	dynamicsWorld->addRigidBody(bt);
	physicsAccessors.insert(std::pair<std::string, btRigidBody*>(btname, bt));
}