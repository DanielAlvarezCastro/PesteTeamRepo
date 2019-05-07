#include "GameGUI.h"



GameGUI::GameGUI(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	GUIMgr = GUIManager::instance();
	healthBar = GUIMgr->getImage("HealthBar");
	gameOver = GUIMgr->getImage("GameOver");
	missionA = GUIMgr->getImage("MissionAccomplished");
}


GameGUI::~GameGUI()
{
}

void GameGUI::Update(float t)
{
	//Deber�a recibir el mensaje cuando la vida se modifique, y cambiar de textura a amarillo o rojo seg�n la cantidad
}

void GameGUI::reciveMsg(Message * msg)
{
	if (msg->id == "MISSION_ACCOMPLISHED") {
		missionA->setVisible(true);
	}
	else if (msg->id == "NEXT_LEVEL") {
		GUIMgr->destroyWidget("NextLevel");
		GUIMgr->destroyWidget("GameOver");
		GUIMgr->destroyWidget("HealthBar");
	}
	else if (msg->id == "GAME_OVER")
	{//Cuando reciba el mensaje de que el juego ha terminado 
	//Activa el GUI de GAME_OVER y su booleano
		gameOver->setVisible(true);
		GameOverMsg* goMsg = static_cast<GameOverMsg*>(msg);
		
		
	}
}
