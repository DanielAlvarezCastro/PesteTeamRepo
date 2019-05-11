#include "GameManager.h"
#include "Messages.h"
#include "GameSceneManager.h"
#include <string>

int GameManager::currentLevel = 1;

GameManager::GameManager(GameObject* go, int _maxLevel) : BehaviourComponent(go), maxLevel(_maxLevel)
{
	GameOver = false;
	nextLevel = false;
	nextLevelMaxTime = 5.0;
	nextLevelTimer = nextLevelMaxTime;
}

void GameManager::Update(float t)
{
	if (nextLevel) {
		nextLevelTimer -= t;
		if (nextLevelTimer <= 0) {
		
			std::string sceneName;
			sceneName = "Scene" + to_string(currentLevel);
			GameSceneManager::instance()->ChangeToNewScene(sceneName);
		}
	}
	else if (GameOver) {
		nextLevelTimer -= t;
		if (nextLevelTimer <= 0) {
			GameSceneManager::instance()->ReturnToScene("MainMenu");
			//GameSceneManager::instance()->CloseScene();
		}
	}
}

void GameManager::reciveMsg(Message * msg)
{
	
	//Activa el booleano que activa la cuenta atrás para cambiar de escena
	if (msg->id == "MISSION_ACCOMPLISHED") {
		GameOver = false;
		if (currentLevel == maxLevel) {

		}
		else {
			currentLevel++;
			nextLevel = true;
		}
	}
	else if (msg->id == "GAME_OVER")
	{//Cuando reciba el mensaje de que el juego ha terminado 
	//Activa el GUI de GAME_OVER y su booleano
		if (!nextLevel) {
			GameOverMsg* goMsg = static_cast<GameOverMsg*>(msg);
			GameOver = true;
			currentLevel = 1;
		}
	}
}


GameManager::~GameManager()
{
}
