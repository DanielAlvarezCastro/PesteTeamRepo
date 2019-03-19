#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <SceneLoader.h>
#include "PlayerController.h"
#include "GameSceneManager.h"

int main() {

	MainApp::instance()->initApp();

	//Carga los recursos del juego las escenas y los gameobjects
	if (!GameSceneManager::instance()->LoadGame()) {
		return false;
	}
	//Carga la escena número 1 ESTÁ DE PRUEBA
	GameSceneManager::instance()->LoadScene("Scene1");
	
	MainApp::instance()->runGame();
	
	return 0;
}