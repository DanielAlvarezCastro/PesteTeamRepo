#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	SoundManager::instance()->Drop();
}

void Scene::createScene(String sceneName)
{
	mSceneMgr = MainApp::instance()->getRoot()->createSceneManager();
	mWindow = MainApp::instance()->getRenderWindow();
	//mSceneMgr->setSkyDome(true, "Test/SpaceDome",5, 1.0);
	mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox", 10);

	SoundManager::instance()->Init();
	intro_ = SoundManager::instance()->PlaySound2D("megaSynthWaveTrack_Intro.mp3", false, false);
}

void Scene::updateScene()
{
	Physics::getInstance()->updatePhysics(MainApp::instance()->deltaTime());

	for (BasicComponent* c : components)
	{
		c->Update(MainApp::instance()->deltaTime());
	}

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->closeApp();
	}

	if (intro_->isFinished() && !playing_music_) {
		body_ = SoundManager::instance()->PlaySound2D("megaSynthWaveTrack_Body.mp3", true, false);
		playing_music_ = true;
	}
}

