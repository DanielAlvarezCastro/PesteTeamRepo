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
	//esto debe sustituirse por actualizar los componentes
	for (GameObject* go : gameObjects)
	{
	}
	for (BasicComponent* c : components)
	{
		c->Update();
	}

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->closeApp();
	}
}

void Scene::createCamera(String cameraName)
{

	mCamera = mSceneMgr->createCamera(cameraName);

	mCamera->setNearClipDistance(5);

	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("n" + cameraName);
	mCamNode->attachObject(mCamera);
	mCamNode->setPosition(0, 0, 80);
	mCamNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::Node::TS_WORLD);

	
	

}

void Scene::createLight(String lightName)
{
	Ogre::Light* luz = mSceneMgr->createLight(lightName);
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);
	Ogre::SceneNode* mLightNode = mCamNode->createChildSceneNode("n" + lightName);
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, 0, -1));
}
