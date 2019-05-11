#include "GameSceneManager.h"
#include "GUIManager.h"
#include <Physics.h>

GameSceneManager* GameSceneManager::instance_ = nullptr;
GameSceneManager::GameSceneManager()
{
}


GameSceneManager::~GameSceneManager()
{
	delete loader;
}
void GameSceneManager::setPlayerShip(string shipName)
{
	loader->setPlayerShip(shipName);
}
//Carga las escenas a trav�s del SceneLoader que lee los json
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
	Physics::getInstance()->releasePhysics();
	Physics::getInstance()->initPhysics();
	if (loader->sceneAlreadyLoaded(sceneName)) {
		Scene* escena = loader->loadSceneFromMemory(sceneName);
		escena->showGUI();
	}
	else {
		Scene* escena = new Scene();
		MainApp::instance()->AddScene(escena);
		if (!loader->loadSceneFromFile(sceneName, escena))
			return false;
	}
	
}
//Borra la escena actual y carga una nueva
void GameSceneManager::ChangeToNewScene(string nextScene)
{
	//Esconde el GUI de la escena anterior
	MainApp::instance()->getCurrentScene()->hideGUI();
	string sceneName = MainApp::instance()->getCurrentScene()->getName();
	//Borra de memoria la escena
	loader->deleteScene(sceneName);
	//La quita de la pila de escenas
	MainApp::instance()->QuitScene();
	LoadScene(nextScene);
}
//Borra las escenas necesarias hasta llegar a la indicada y cargarla
void GameSceneManager::ReturnToScene(string sceneName)
{
	//Esconde el GUI de la escena anterior
	string currentScene = MainApp::instance()->getCurrentScene()->getName();
	MainApp::instance()->getCurrentScene()->hideGUI();
	do {
		//Borra de memoria la escena
		loader->deleteScene(currentScene);
		//La quita de la pila de escenas
		MainApp::instance()->QuitScene();
		currentScene = MainApp::instance()->getCurrentScene()->getName();
	} while (currentScene != sceneName);
	LoadScene(sceneName);
}
//Cierra la escena actual y carga la anterior en la pila
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
