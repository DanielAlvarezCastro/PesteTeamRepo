#include"Physics.h"

Physics* Physics::instance_ = nullptr;

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

void Physics::trackAndChangeNameOfRigidBody(std::string oName_, std::string nName_) {
	//buscamos el valor con la antigua clave
	const std::map<std::string,btRigidBody*>::iterator it = physicsAccessors.find(oName_);
	//si la encontramos
	if (it != physicsAccessors.end()) {
		//sacamos el valor
		btRigidBody* aux = it->second;
		//borramos la entrada del diccionario
		physicsAccessors.erase(it);
		//y metemos una entrada con la nueva pareja clave-valor
		physicsAccessors.insert(std::pair<std::string, btRigidBody*>(nName_, aux));
	}
}

std::string Physics::getRigidBodyName(btRigidBody* rb) {
	bool flag = false;
	//buscamos en todo el diccionario
	std::map<std::string, btRigidBody*>::iterator it = physicsAccessors.begin();	
	while (it != physicsAccessors.end() && !flag) {
		//si coincide
		if (it->second == rb)
			flag = true;
		else it++;
	}
	if (flag)
		return it->first;
	//si entra significa que el elemento no esta en el diccionario
	else return NULL;
}

btRigidBody* Physics::getRigidBodyFromName(std::string name) {
	return physicsAccessors.at(name);
}