#pragma once
#include <Ogre.h>
#include "MainApp.h"

class Scene
{
protected:
	MainApp* APPLICATION;

	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

	Ogre::Camera* mCamera;
	Ogre::Viewport* vp;
	Ogre::SceneNode* mCamNode;

	Ogre::SceneNode* gameObject;

public:
	Scene(MainApp* app);
	~Scene();
	void createScene();
	void updateScene();
};

