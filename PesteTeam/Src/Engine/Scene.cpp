#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::createScene(String sceneName)
{
	mSceneMgr = MainApp::instance()->getRoot()->createSceneManager();
	mWindow = MainApp::instance()->getRenderWindow();
	mSceneMgr->setSkyDome(true, "Test/SpaceDome",5, 1.0);
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

