#pragma once
#include <Ogre.h>

#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h> 
#include "windows.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <OgreFileSystemLayer.h>
#include <OgreRenderTarget.h>
#include <OISException.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>
//#include "InputManager.h"
//#include "Scene.h"
//#include "SceneMachine.h"

using namespace Ogre;
using namespace OIS;

class SceneMachine;

class MainApp
{
private:
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::RenderSystem* renderSys;
	Ogre::SceneManager* mSceneMgr;

	SceneMachine* sceneMC;

	OIS::InputManager* mInputMgr;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	bool appRunning;

	void messagePump();

public:	
	MainApp();
	~MainApp();
	int initApp();
	int initOgre();
	void initOIS();
	void locateResources();
	void closeApp();

	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {}

	//getters
	Ogre::RenderWindow* getRenderWindow() { return mWindow; }
	Ogre::RenderSystem* getRenderSys() { return renderSys; }
	Ogre::SceneManager* getSceneMgr() { return mSceneMgr; }

	OIS::Mouse* getMouse() { return mMouse; }
	OIS::Keyboard* getKeyboard() { return mKeyboard; }
};
