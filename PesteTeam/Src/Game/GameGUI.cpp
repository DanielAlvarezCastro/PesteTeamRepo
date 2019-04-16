#include "GameGUI.h"



GameGUI::GameGUI(GameObject* gameObject) : BehaviourComponent(gameObject)
{
	GUIMgr = GUIManager::instance();
	healthBar = GUIMgr->getImage("HealthBar");
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
}
