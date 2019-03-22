#pragma once
#include <Ogre.h>
#include "GameObject.h"

class Scene
{
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCamNode;

	std::vector<GameObject*> gameObjects;
	std::vector<BasicComponent*> components;


public:
	Scene();
	~Scene();
	void createScene(String sceneName);
	void updateScene();

	void addGameObject(GameObject* GO) { gameObjects.push_back(GO); };
	void addComponent(BasicComponent* GO) { components.push_back(GO); };
	void createCamera(String cameraName);
	Ogre::Camera* getCamera() { return mCamera; };
	Ogre::SceneManager* getSceneManager() { return mSceneMgr; };
	void createLight(String lightName);
};

