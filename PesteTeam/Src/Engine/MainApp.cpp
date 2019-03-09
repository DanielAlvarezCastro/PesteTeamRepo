#include "MainApp.h"
#include <OgreConfigFile.h>
#include <iostream>
#include "SceneMachine.h"

MainApp::MainApp() : mRoot(0), mResourcesCfg(Ogre::BLANKSTRING), mPluginsCfg(Ogre::BLANKSTRING)
{
	sceneMC = new SceneMachine();
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
	sceneMC->addScene(new Scene(this));

	appRunning = true;
	while (appRunning) {
		messagePump();

		mWindow->update();
		if (mWindow->isClosed()) return false;

		mKeyboard->capture();
		mMouse->capture();
		
		if (!mRoot->renderOneFrame()) return false;
		sceneMC->update();
	}
	return 1;
}
void MainApp::messagePump()
{
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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

	Ogre::RenderSystemList rsl = mRoot->getAvailableRenderers();
	renderSys = *rsl.begin();
	mRoot->setRenderSystem(renderSys);

	// initialise root
	// mRoot->initialise(false);
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
	OIS::ParamList paramList;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

	mInputMgr = OIS::InputManager::createInputSystem(paramList);

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

void MainApp::closeApp()
{
	appRunning = false;
}