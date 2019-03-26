#include<btBulletDynamicsCommon.h>
#include<vector>
#include<map>

#ifndef _PHYSICS_H
#define _PHYSICS_H

class Physics {
private:
	static Physics* instance_;
protected:
	//elementos necesarios para la configuracion inicial
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	//el propio mundo fisico
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
public:
	Physics();
	~Physics();
	void initPhysics();

	void addRigidBodyToWorld(btRigidBody* bt,const std::string & btname);

	btDynamicsWorld* getDynamicWorld() { return dynamicsWorld; };
	std::vector<btCollisionShape*> getCollisionShapes() { return collisionShapes; };

	static Physics* getInstance() {
		if (instance_ == nullptr)
			instance_ = new Physics();
		return instance_;
	}
};

#endif