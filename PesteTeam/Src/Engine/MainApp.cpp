#include "MainApp.h"
#include <OgreConfigFile.h>
#include <iostream>


MainApp::MainApp() : mRoot(0), mResourcesCfg(Ogre::BLANKSTRING), mPluginsCfg(Ogre::BLANKSTRING)
{
}

MainApp::~MainApp()
{
	delete mRoot;
}

int MainApp::initApp() 
{
	
	initOgre();
	initOIS();
	mSceneMgr = mRoot->createSceneManager();

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

	Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);
	ogreNode->setScale(0.1, 0.1, 0.1);

	Light* luz = mSceneMgr->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(0.75, 0.75, 0.75);
	//mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
	SceneNode* mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(luz);

	mLightNode->setDirection(Ogre::Vector3(-1, 0, -1));
	appRunning = true;
	while (appRunning) {
		
		mKeyboard->capture();
		mMouse->capture();
		if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
			appRunning = false;
		}
		
		if (mWindow->isClosed()) {
			return false;
		}
		if (!mRoot->renderOneFrame())
			return false;
		ogreNode->setPosition(ogreNode->getPosition().x + 1, ogreNode->getPosition().y, ogreNode->getPosition().z);
		
	}
}

int MainApp::initOgre()
{
	//INICIALIZACION DE OGRE
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	mRoot = new Ogre::Root(mPluginsCfg);

	if (mRoot == NULL)
	{
		return 0;
	}

	renderSys = *mRoot->getAvailableRenderers().begin();
	mRoot->setRenderSystem(renderSys);

	// initialise root
	//mRoot->initialise(false);
	locateResources();

	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog(NULL)))
		return false;

	renderSys->setConfigOption("Full Screen", "No");
	renderSys->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	mWindow = mRoot->initialise(true, "Retrowave Spaceship FoxMcDonald");

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MainApp::initOIS()
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputMgr = OIS::InputManager::createInputSystem(pl);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, false));
}

void MainApp::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
	Ogre::String sec, type, name;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			name = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type, sec);
		}
	}
}
