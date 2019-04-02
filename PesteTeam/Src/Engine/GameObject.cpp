#include "GameObject.h"
#include <OgreSceneManager.h>
#include "Scene.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::release()
{
	references--;
	if (references == 0) 
	{
		if(rigidBody) delete rigidBody;
		if (camera) delete camera;
		if (light) delete light;
		for (auto &bComponent : behaviourComponents)
		{
			delete bComponent;
		}
		//^Delete dynamic objects^
		delete this;
	}
}

void GameObject::calculateDirection()
{
	Ogre::Quaternion q = ogreNode->getOrientation();
	Ogre::Vector3 dir= Ogre::Vector3::ZERO;
	dir.x = 2 * (q.x * q.z - q.w* q.y);
	dir.y = -2 * (q.y * q.z - q.w * q.x);
	dir.z = -1*(1- 2 * (q.x * q.x - q.y * q.y));
	transform.direction = dir;
}

Vec3 GameObject::getBoundingBox()
{
	Ogre::AxisAlignedBox auxAABB = ogreEntity->getWorldBoundingBox(true);
	Vec3 bb = auxAABB.getSize();
	ogreNode->showBoundingBox(true);
	return bb;
}

void GameObject::reciveMsg(Message * msg)
{
	if (rigidBody) rigidBody->reciveMsg(msg);
	for (auto &bComponent: behaviourComponents)
	{
		bComponent->reciveMsg(msg);
	}
}

void GameObject::asingFather(GameObject * father_)
{
	father = father_;
	ogreNode->getParent()->removeChild(ogreNode);
	father->reciveChild(ogreNode);
}
void GameObject::createEntity(std::string mesh, std::string name_, Scene* scene)
{
	name = name_;
	Ogre::SceneManager* scnMgr = scene->getSceneManager();
	ogreEntity = scnMgr->createEntity(mesh);

	ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(ogreEntity);
	ogreNode->setScale(0.1, 0.1, 0.1);
}

void GameObject::createEmptyEntity(std::string name_, Scene * scene)
{
	name = name_;
	Ogre::SceneManager* scnMgr = scene->getSceneManager();

	ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
}

void GameObject::addRigidbody(RigidBody * rb)
{
	rigidBody = rb;
}

void GameObject::attachCamera(Ogre::Camera * cam)
{
	camera = cam;
	ogreNode->attachObject(cam);
}

void GameObject::attachLight(Ogre::Light * lig)
{
	light = lig;
	ogreNode->attachObject(light);

	ogreNode->setDirection(Ogre::Vector3(-1, 0, -1));
}
