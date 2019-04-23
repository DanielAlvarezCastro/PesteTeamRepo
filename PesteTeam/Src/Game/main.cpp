#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <SceneLoader.h>
#include "PlayerController.h"
#include "GameSceneManager.h"
#include <SoundManager.h>

int main() {

	srand(time(NULL));

	MainApp::instance()->initApp();

	//Carga los recursos del juego las escenas y los gameobjects
	if (!GameSceneManager::instance()->LoadGame()) 
		return false;

	//Carga la escena n�mero 1 EST� DE PRUEBA
	GameSceneManager::instance()->LoadScene("MainMenu");

	SoundManager::instance()->Init();

	MainApp::instance()->runGame();

	SoundManager::instance()->Drop();
	
	return 0;
}