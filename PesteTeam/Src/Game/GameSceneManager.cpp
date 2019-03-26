#include "GameSceneManager.h"


GameSceneManager* GameSceneManager::instance_ = nullptr;
GameSceneManager::GameSceneManager()
{
}


GameSceneManager::~GameSceneManager()
{
}
//Carga las escenas a través del SceneLoader que lee los json
bool GameSceneManager::LoadGame()
{
	loader = new SceneLoader("./Assets/Scenes/");
	if (!loader->loadPrefabsFromFile())
		return false;
	//if (!loader->loadSceneFromFile("Scene1"))
		//return false;
	if (!loader->loadTestScene())
		return false;
	return true;
}

//Carga una escena con un nombre determinado
void GameSceneManager::LoadScene(string sceneName)
{
	Scene* escena = loader->getScene(sceneName);
	MainApp::instance()->setupViewport(escena->getCamera());
	MainApp::instance()->AddScene(escena);
}
