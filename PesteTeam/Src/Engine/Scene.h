#pragma once
#include <Ogre.h>
#include "GameObject.h"

class Scene
{
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

	Ogre::Camera* mCamera;
	Ogre::Viewport* vp;
	Ogre::SceneNode* mCamNode;

	std::vector<GameObject*> gameObjects;

public:
	Scene();
	~Scene();
	void createScene();
	void updateScene();
};

