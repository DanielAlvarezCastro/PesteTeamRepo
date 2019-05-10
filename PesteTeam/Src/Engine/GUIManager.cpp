#include "GUIManager.h"
#include "MainApp.h"

GUIManager* GUIManager::instance_ = nullptr;

void GUIManager::initScene(Scene* escena)
{	
	mPlatform->getRenderManagerPtr()->setSceneManager(escena->getSceneManager());
	
}

void GUIManager::createTextbox(string text, int x, int y, int w, int h, string skinName, string name, int fontHeight)
{
	MyGUI::TextBox* t = mGUI->createWidget<MyGUI::TextBox>(skinName, MyGUI::IntCoord(x, y, w, h), MyGUI::Align::Default, "Main", name);
	t->setCaption(text);
	t->setFontHeight(fontHeight);
}

MyGUI::ImageBox* GUIManager::createImage(string fileName, int x, int y, int w, int h, string skinName, string name)
{
	MyGUI::ImageBox* image = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(x,y,w,h), MyGUI::Align::Default, "Main", name);
	image->setImageTexture(fileName);
	return image;
}

MyGUI::TextBox * GUIManager::getTextBox(string name)
{
	return mGUI->findWidget<MyGUI::TextBox>(name);
}
MyGUI::ImageBox * GUIManager::getImage(string name)
{
	return mGUI->findWidget<MyGUI::ImageBox>(name);
}

GUIManager::GUIManager()
{
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(MainApp::instance()->getRenderWindow(), MainApp::instance()->getCurrentSceneMgr()); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();
	MyGUI::PointerManager::getInstance().setVisible(false);
	
}

void GUIManager::destroyWidget(string name) {
	//MyGUI::Widget* w = mGUI->findWidgetT(name);
	MyGUI::ImageBox* w = mGUI->findWidget<MyGUI::ImageBox>(name);
	mGUI->destroyWidget(w);
}

GUIManager::~GUIManager()
{
	mGUI->shutdown();
	delete mGUI;
	mGUI = 0;
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;
}
