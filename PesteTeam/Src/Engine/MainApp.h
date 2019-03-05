#pragma once
#include <Ogre.h>

#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h> 
#include "windows.h"
#include <OgreFileSystemLayer.h>

using namespace Ogre;

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
public:	
	MainApp();
	~MainApp();
	int initApp();
	void locateResources();
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {}
};
