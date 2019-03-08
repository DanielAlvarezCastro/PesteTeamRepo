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

using namespace Ogre;
using namespace OIS;

class MainApp
{
private:
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::RenderSystem* renderSys;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* vp;
	Ogre::SceneNode* mCamNode;
	OIS::InputManager* mInputMgr;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;
	HWND hWnd;
	bool appRunning;

public:	
	MainApp();
	~MainApp();
	int initApp();
	int initOgre();
	void initOIS();
	void locateResources();
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {}
};
