#include "GUIManager.h"
#include "MainApp.h"

GUIManager* GUIManager::instance_ = nullptr;

void GUIManager::initScene(Scene* escena)
{	
	mPlatform->getRenderManagerPtr()->setSceneManager(escena->getSceneManager());
	
}

void GUIManager::createTextbox(string text, int x, int y, int w, int h, string skinName, string name, int size)
{
	MyGUI::TextBox* t = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>(skinName, MyGUI::IntCoord(x, y, w, h), MyGUI::Align::Default, "Main", name);
	t->setCaption(text);
	t->setFontHeight(size);
}

MyGUI::TextBox * GUIManager::getTextBox(string name)
{
	return MyGUI::Gui::getInstance().findWidget<MyGUI::TextBox>("ScoreTextBox");
}

GUIManager::GUIManager()
{
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(MainApp::instance()->getRenderWindow(), MainApp::instance()->getCurrentSceneMgr()); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();
	MyGUI::PointerManager::getInstance().setVisible(false);
}


GUIManager::~GUIManager()
{
}
