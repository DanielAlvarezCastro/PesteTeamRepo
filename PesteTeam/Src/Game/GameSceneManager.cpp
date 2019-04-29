#include "GameSceneManager.h"
#include "GUIManager.h"

GameSceneManager* GameSceneManager::instance_ = nullptr;
GameSceneManager::GameSceneManager()
{
}


GameSceneManager::~GameSceneManager()
{
}
void GameSceneManager::setPlayerShip(string shipName)
{
	loader->setPlayerShip(shipName);
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
	if (loader->sceneAlreadyLoaded(sceneName)) {
		escena = loader->loadSceneFromMemory(sceneName, escena);
	}
	else {
		MainApp::instance()->AddScene(escena);
		if (!loader->loadSceneFromFile(sceneName, escena))
			return false;
	}
	escena->showGUI();
}

void GameSceneManager::CloseScene()
{
	//Esconde el GUI de la escena anterior
	MainApp::instance()->getCurrentScene()->hideGUI();
	string sceneName = MainApp::instance()->getCurrentScene()->getName();
	//Borra de memoria la escena
	loader->deleteScene(sceneName);
	//La quita de la pila de escenas
	MainApp::instance()->QuitScene();
	//Carga la siguiente escena en la pila
	Scene* scene = MainApp::instance()->getCurrentScene();
	string nextSceneName = scene->getName();
	LoadScene(nextSceneName);
}
