#include "GameSceneManager.h"
#include "GUIManager.h"

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
	/*if (!loader->loadSceneFromFile("MainMenu"))
		return false;
	if (!loader->loadSceneFromFile("Scene1"))
		return false;
	if (!loader->loadSceneFromFile("Scene2"))
		return false;
	if (!loader->loadTestScene())
		return false;*/
	return true;
}

//Carga una escena con un nombre determinado
bool GameSceneManager::LoadScene(string sceneName)
{
	Scene* escena = new Scene();
	MainApp::instance()->AddScene(escena);	
	if (!loader->loadSceneFromFile(sceneName, escena))
		return false;
	
}
