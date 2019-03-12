#include "Scene.h"

Scene::Scene()
{
	createScene();
}

Scene::~Scene()
{
}

void Scene::createScene() 
{
	mSceneMgr = MainApp::instance()->getSceneMgr();
	mWindow = MainApp::instance()->getRenderWindow();

	mCamera = mSceneMgr->createCamera("MainCam");

	mCamera->setNearClipDistance(5);

	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(mCamera);
	mCamNode->setPosition(0, 0, 80);
	mCamNode->lookAt(Ogre::Vector3(0, 0, -300), Ogre::Node::TS_WORLD);

	vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	Ogre::Entity* ogreEntity = mSceneMgr->createEntity("cube.mesh");

	gameObject = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	gameObject->attachObject(ogreEntity);
	gameObject->setScale(0.1, 0.1, 0.1);

	Ogre::Light* luz = mSceneMgr->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);
	//mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
	Ogre::SceneNode* mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, 0, -1));
}

void Scene::updateScene() 
{
	gameObject->setPosition(gameObject->getPosition().x + 1, gameObject->getPosition().y, gameObject->getPosition().z);

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->closeApp();
	}
}