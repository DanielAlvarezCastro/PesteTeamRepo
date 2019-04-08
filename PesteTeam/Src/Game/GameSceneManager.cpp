#include "GameSceneManager.h"
#include "GUIManager.h"

GameSceneManager* GameSceneManager::instance_ = nullptr;
GameSceneManager::GameSceneManager()
{
}


GameSceneManager::~GameSceneManager()
{
}
//Carga las escenas a trav�s del SceneLoader que lee los json
bool GameSceneManager::LoadGame()
{
	loader = new SceneLoader("./Assets/Scenes/");
	if (!loader->loadPrefabsFromFile())
		return false;
	/*if (!loader->loadSceneFromFile("Scene1"))
		return false;
	if (!loader->loadSceneFromFile("Scene2"))
		return false;*/
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
	GUIManager::instance()->initScene(escena);
	GUIManager::instance()->createTextbox("Bueno dia, despierta con buena actitud", 0, 0, 500, 100, "TextBox", "TestTextBox", 30);
	
}
