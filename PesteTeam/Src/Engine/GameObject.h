#include <Ogre.h>
#include <vector>
#include "RigidBody.h"
#include "BehaviourComponent.h"
#include "MainApp.h"

#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H
class Scene;
struct Vec3 
{
	float x;
	float y;
	float z;

	Vec3(float _x=0, float _y=0, float _z=0) : x(_x), y(_y), z(_z){};
	Vec3(Ogre::Vector3 vec): x(vec.x), y(vec.y), z(vec.z) {};
	Ogre::Vector3 getVector() { return Ogre::Vector3((Ogre::Real)x, (Ogre::Real)y, (Ogre::Real)z); };
};

class GameObject
{
#pragma region GO Private Atributes
private:
	struct Transform
	{
#pragma region Atributes
		Ogre::Vector3 position; //Posicion en el espacio
		Ogre::Vector3 direction; //Direccion de la entidad
		Ogre::Vector3 scale; //Tamaño del objeto
#pragma endregion
		//Inializaciones por defecto del trasform
		Transform(Ogre::Vector3 position_ = Ogre::Vector3::ZERO, Ogre::Vector3 direction_ = Ogre::Vector3::ZERO,
			Ogre::Vector3 rotation_ = Ogre::Vector3::ZERO, Ogre::Vector3 scale_ = Ogre::Vector3::ZERO) :
			position(position_), direction(direction_), scale(scale_) {};

	}transform;
	GameObject* father = NULL;
	Ogre::Camera* camera;
	Ogre::Light* light;
	RigidBody* rigidBody = nullptr;
	std::vector<BehaviourComponent*>behaviourComponents;
	int references = 0;
	bool active = true;
	//Ogre scene node
	Ogre::SceneNode* ogreNode = nullptr;
	//Ogre Entity
	Ogre::Entity* ogreEntity = nullptr;
	std::string name;
#pragma endregion

public:
	GameObject();
	~GameObject();
	void addReference() { references++; };
	void release();
	void setActive(bool active_) { active = active_; };
	void reciveMsg(Message* msg);

	void asingFather(GameObject* father_);
	Scene* getScene() { return MainApp::instance()->getCurrentScene(); };
	void reciveChild(Ogre::SceneNode* childNode) { ogreNode->addChild(childNode); };
	void createEntity(std::string mesh, std::string name_, Scene* scene = MainApp::instance()->getCurrentScene());
	void createEmptyEntity(std::string name_, Scene* scene = MainApp::instance()->getCurrentScene());
	void addRigidbody(RigidBody* rb);
	void attachCamera(Ogre::Camera* cam);
	void attachLight(Ogre::Light* lig);
	void setMaterial(std::string materialName);
	string getMeshName();

#pragma region Transform functions
#pragma region Trasform Getters
	Ogre::Quaternion getOrientation() { return ogreNode->getOrientation(); };
	Vec3 getGlobalPosition() { return ogreNode->_getDerivedPosition(); }
	std::string getName() { return name; };
	Ogre::Vector3 getPosition() { return transform.position; };
	Ogre::Vector3 getDirection() { return transform.direction; };
	Vec3 getScale() { return Vec3(transform.scale); };
	float getRoll() { return ogreNode->getOrientation().getRoll().valueRadians(); }
	float getPitch() { return (float)ogreNode->getOrientation().getPitch().valueRadians(); }
	float getYaw() { return (float)ogreNode->getOrientation().getYaw().valueRadians(); }
#pragma endregion
#pragma region Transform Setters
	void setPosition(Vec3 position_) {transform.position = position_.getVector(); ogreNode->setPosition(transform.position); };
	void setDirection(Vec3 direction_) { transform.direction = direction_.getVector(); ogreNode->setDirection(transform.direction); };
	void setScale(Vec3 scale_) { transform.scale = scale_.getVector(); ogreNode->setScale(transform.scale); };
	void setInitialState() { ogreNode->setInitialState(); };
#pragma endregion
#pragma region Position Methods
	void translate(Vec3 movement) { ogreNode->translate(movement.getVector()); setPosition(ogreNode->getPosition()); };
#pragma endregion
#pragma region Rotation Methods
	void calculateDirection();
	void yaw(float radian) { ogreNode->yaw(Ogre::Radian((Ogre::Real)radian)); calculateDirection(); };
	void pitch(float radian) { ogreNode->pitch(Ogre::Radian((Ogre::Real)radian)); calculateDirection(); };
	void roll(float radian) { ogreNode->roll(Ogre::Radian((Ogre::Real)radian)); calculateDirection();};
	void rotate(Vec3 axis, float radian) { ogreNode->rotate(axis.getVector(), Ogre::Radian((Ogre::Real)radian)); calculateDirection();};
	void lookAt(Vec3 position, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_WORLD) {
		ogreNode->lookAt(position.getVector(), relativeTo); calculateDirection();;
	}
	void setOrientation(Ogre::Quaternion q) { ogreNode->setOrientation(q); };
#pragma region Bounding Box Methods
	Vec3 getBoundingBox();
#pragma endregion

#pragma region Rigid Body Methods
	RigidBody* getRigidBody() { return rigidBody; };
#pragma endregion

#pragma endregion
#pragma endregion
};

#endif // _GAMEOBJECT



