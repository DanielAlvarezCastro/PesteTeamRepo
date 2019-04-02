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
	   
}

void Scene::updateScene()
{
	//puntero auxiliar para ahorrar llamadas
	btDynamicsWorld* auxWolrd = Physics::getInstance()->getDynamicWorld();
	if (auxWolrd != NULL) {
		//actualizamos el tiempo
		auxWolrd->stepSimulation(MainApp::instance()->deltaTime());
	}

	for (BasicComponent* c : components)
	{
		c->Update(MainApp::instance()->deltaTime());
	}

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->closeApp();
	}
}

