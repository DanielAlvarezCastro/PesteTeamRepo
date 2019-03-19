#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <SceneLoader.h>
#include "PlayerController.h"
#include "GameSceneManager.h"
#include <SoundManager.h>

int main() {

	MainApp::instance()->initApp();

	//Carga los recursos del juego las escenas y los gameobjects
	if (!GameSceneManager::instance()->LoadGame()) {
		return false;
	}
	//Carga la escena número 1 ESTÁ DE PRUEBA
	GameSceneManager::instance()->LoadScene("Scene1");
	
	SoundManager::instance()->Init();
	SoundManager::instance()->LoadSound("./Assets/Audio/yell.mp3");
	SoundManager::instance()->PlaySoundA("./Assets/Audio/yell.mp3", SoundVector{ 0, 0, 0 }, SoundManager::instance()->VolumeTodb(1.0f));

	MainApp::instance()->runGame();
	
	return 0;
}