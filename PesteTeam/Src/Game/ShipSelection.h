#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>
#include "Messages.h"
#include "GameSceneManager.h"
class ShipSelection :
	public BehaviourComponent
{
private:
	GUIManager* GUIMgr = nullptr;
	//States
	int state = 0;// Cada uno por nave
	void handleStates();
	//Buffer
	OIS::KeyCode lastKey;
	double inputBufferTimer = 0.2;
	double lastTimePressed = 0;
	OIS::Keyboard* keyboard;
	std::vector<GameObject*> ships;
	std::vector<string> meshes;
	int shipsNum=0;
	float distance = 0;
	float distanceBetweenShips = 0;
	GameObject* shipsPivot;
	void shipsAnimation();
	int direction = 0;//0 no se está moviendo, -1 mueve a la izquierda y 1 a la derecha
	float currentPos = 0.0;
	float speed = 0;

	//Timer para que haya un delay al cargar la pantalla
	float initTimer = 0.0;
	float currentTime = 0.0;
public:
	void selectShip();
	void addShipMesh(string mesh);
	void addShipModel(GameObject* go);
	ShipSelection(GameObject* gameObject, float shipDistance, GameObject* pivot);
	void setInitialShipsPosition();
	virtual ~ShipSelection();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

