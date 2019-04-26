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

	//Carga la escena número 1 ESTÁ DE PRUEBA
	GameSceneManager::instance()->LoadScene("MainMenu");

	SoundManager::instance()->Init();
	SoundManager::instance()->GetEngine()->stopAllSounds();
	SoundManager::instance()->PlaySound2D("SynthSong0.mp3", true, false);

	MainApp::instance()->runGame();

	SoundManager::instance()->Drop();
	
	return 0;
}