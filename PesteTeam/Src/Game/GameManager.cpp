#include "GameManager.h"
#include "Messages.h"


GameManager::GameManager(GameObject* go) : BehaviourComponent(go)
{
	GameOver = false;
	missionA = false;
	currentLevel = 1;
}

void GameManager::Update(float t)
{
	//Espera input para pasar a NextLevel
	if (missionA) {

	}
	//Espera input para volver al MainMenu
	if (GameOver) {

	}
}

void GameManager::reciveMsg(Message * msg)
{
	
	if (msg->id == "MISSION_ACCOMPLISHED") {
		currentLevel++;
		GameOver = false;
	}
	else if (msg->id == "NEXT_LEVEL") {
		currentLevel++;
		GameOver = false;
	}
	else if (msg->id == "GAME_OVER")
	{//Cuando reciba el mensaje de que el juego ha terminado 
	//Activa el GUI de GAME_OVER y su booleano
		GameOverMsg* goMsg = static_cast<GameOverMsg*>(msg);
		GameOver = true;
	}
}


GameManager::~GameManager()
{
}
