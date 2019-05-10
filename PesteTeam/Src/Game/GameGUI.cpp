#include "GameGUI.h"



GameGUI::GameGUI(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	GUIMgr = GUIManager::instance();
	healthBar = GUIMgr->getImage("HealthBar");
	gameOver = GUIMgr->getImage("GameOver");
	missionA = GUIMgr->getImage("MissionAccomplished");
	warning = GUIMgr->getImage("Warning");
}


GameGUI::~GameGUI()
{
}

void GameGUI::Update(float t)
{
	//Debería recibir el mensaje cuando la vida se modifique, y cambiar de textura a amarillo o rojo según la cantidad
}

void GameGUI::reciveMsg(Message * msg)
{
	if (msg->id == "UPDATE_HEALTHBAR") {
		UpdateHealthBarMsg* uMsg = static_cast<UpdateHealthBarMsg*>(msg);
		int currentH = uMsg->currentHealth;
		int w = healthbarFullW*currentH / fullHealth;
		healthBar->setSize(w, 40);
	}
	else if (msg->id == "MISSION_ACCOMPLISHED") {
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
		cout << "GAMEOVER" << endl;
		gameOver->setVisible(true);	
	}
	else if (msg->id == "ENTER_WARNING_ZONE")
	{
		warning->setVisible(true);
	}
	else if (msg->id == "EXIT_WARNING_ZONE")
	{
		warning->setVisible(false);
	}
}
