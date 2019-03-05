#pragma once
#include <Ogre.h>

#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h> 
#include "windows.h"

using namespace Ogre;

class MainApp
{
public:	
	MainApp();
	~MainApp();
	int initApp();
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {}
};
