#include "MainApp.h"

MainApp::MainApp()
{
}

MainApp::~MainApp()
{
}

int MainApp::initApp() 
{
	// create root
	Root *root = new Root();
	// choose renderer
	if (root == NULL)
	{
		return 0;
	}
	// initialise root
	root->initialise(false);
	// create main window
	RenderWindow *renderWindow = root->createRenderWindow("Main", 320, 240, false);
	// create the scene
	SceneManager *sceneMgr = root->createSceneManager("SceneManager");
	// add a camera
	Camera *mainCam = sceneMgr->createCamera("MainCam");
	// add viewport
	Viewport *vp = renderWindow->addViewport(mainCam);
}