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
	Root *root; 

#if _DEBUG
	root = new Root("plugins_d.cfg");
#else
	root = new Root("plugins.cfg");
#endif

	root->setRenderSystem(*root->getAvailableRenderers().begin());

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
	//SceneManager *sceneMgr = root->createSceneManager("SceneManager");
	// add a camera
	//Camera *mainCam = sceneMgr->createCamera("MainCam");
	// add viewport
	//Viewport *vp = renderWindow->addViewport(mainCam);
}