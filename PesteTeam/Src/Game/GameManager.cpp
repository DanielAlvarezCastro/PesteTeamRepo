#include "GameManager.h"
#include "Messages.h"
#include "GameSceneManager.h"
#include <string>

int GameManager::currentLevel = 1;

GameManager::GameManager(GameObject* go) : BehaviourComponent(go)
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
			GameSceneManager::instance()->CloseScene(sceneName);
		}
	}
}

void GameManager::reciveMsg(Message * msg)
{
	
	//Activa el booleano que activa la cuenta atrás para cambiar de escena
	if (msg->id == "MISSION_ACCOMPLISHED") {
		currentLevel++;
		GameOver = false;
		nextLevel = true;
	}
	else if (msg->id == "GAME_OVER")
	{//Cuando reciba el mensaje de que el juego ha terminado 
	//Activa el GUI de GAME_OVER y su booleano
		if (!nextLevel) {
			GameOverMsg* goMsg = static_cast<GameOverMsg*>(msg);
			GameOver = true;
		}
	}
}


GameManager::~GameManager()
{
}
