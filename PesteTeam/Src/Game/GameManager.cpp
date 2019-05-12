#include "GameManager.h"
#include "Messages.h"
#include "GameSceneManager.h"
#include <string>

int GameManager::currentLevel = 1;

GameManager::GameManager(GameObject* go, int _maxLevel, float _nextLevelWaitTime) : BehaviourComponent(go), maxLevel(_maxLevel), nextLevelWaitTime(_nextLevelWaitTime)
{
	GameOver = false;
	nextLevel = false;
	nextLevelTimer = nextLevelWaitTime;
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

	if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->getCurrentScene()->hideGUI();
		GameSceneManager::instance()->LoadScene("PauseMenu");
	}
}

void GameManager::reciveMsg(Message * msg)
{
	
	//Activa el booleano que activa la cuenta atr�s para cambiar de escena
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
