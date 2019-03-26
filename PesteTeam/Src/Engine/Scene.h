#pragma once
#include <Ogre.h>
#include "GameObject.h"

class Scene
{
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera* mCamera;

	std::vector<GameObject*> gameObjects;
	std::vector<BasicComponent*> components;


public:
	Scene();
	~Scene();
	void createScene(String sceneName);
	void updateScene();

	void addGameObject(GameObject* GO) { gameObjects.push_back(GO); };
	void addComponent(BasicComponent* GO) { components.push_back(GO); };
	void addCamera(Ogre::Camera* cam) { mCamera = cam; };
	Ogre::Camera* getCamera() {	return mCamera;	};
	Ogre::SceneManager* getSceneManager() { return mSceneMgr; };
};

