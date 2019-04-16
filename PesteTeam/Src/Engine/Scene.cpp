#include "Scene.h"

Scene::Scene()
{
	mSceneMgr = MainApp::instance()->getRoot()->createSceneManager();
	mWindow = MainApp::instance()->getRenderWindow();
}

Scene::~Scene()
{
}

void Scene::createScene(String sceneName)
{	
	name = sceneName;
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 10);
}

void Scene::updateScene()
{
	Physics::getInstance()->updatePhysics(MainApp::instance()->deltaTime());

	for (BasicComponent* c : components)
	{
		c->Update(MainApp::instance()->deltaTime());
	}

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->closeApp();
	}
}

