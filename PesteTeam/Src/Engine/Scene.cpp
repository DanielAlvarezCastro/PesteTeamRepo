#include "Scene.h"
#include "ParticleManager.h"
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

	//for (BasicComponent* c : components)
	//{
	//	c->Update(MainApp::instance()->deltaTime());
	//}
	MainApp::instance()->getParticleManager()->update(MainApp::instance()->deltaTime());
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->isGOActive())
			components[i]->Update(MainApp::instance()->deltaTime());
	}

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->closeApp();
	}
}

void Scene::addGUIObject(MyGUI::Widget* ob)
{
	guiObjects.push_back(ob);
}

void Scene::addGameObject(GameObject* GO) 
{ 
	gameObjects.push_back(GO); 
}

void Scene::addComponent(BasicComponent* bc) 
{ 
	components.push_back(bc); 
}

void Scene::addCamera(Ogre::Camera* cam) 
{ 
	mCamera = cam; 
}