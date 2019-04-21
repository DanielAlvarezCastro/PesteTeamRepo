#include "ShipSelection.h"
#define PI 3.14159265

#include <iostream>

void ShipSelection::setInitialShipsPosition()
{
	//Coloca las naves en el radio de la circunferencia
	for (int i = 0; i < ships.size(); i++) {
		float x = distance * sin(PI*(360 *i/ ships.size()) / 180);
		float y = ships[i]->getPosition().y;
		float z = distance * cos(PI*(360 * i / ships.size()) / 180);
		ships[i]->setPosition(Vec3(x,y,z));
	}
	maxRotation = PI * (360 / shipsNum) / 180;
}

void ShipSelection::shipsAnimation()
{
	for (int i = 0; i < ships.size(); i++) {
		ships[i]->yaw(0.05);
	}
}

void ShipSelection::selectShip()
{
	GameSceneManager::instance()->setPlayerMesh(meshes[state]);
	GameSceneManager::instance()->LoadScene("TestScene");
}

void ShipSelection::addShipMesh(string mesh)
{
	meshes.push_back(mesh);
}

void ShipSelection::addShipModel(GameObject * go)
{
	ships.push_back(go);
	shipsNum++;
}

ShipSelection::ShipSelection(GameObject* gameObject, float shipDistance, GameObject* circle)
	: BehaviourComponent(gameObject), distance(shipDistance), circlePivot(circle)
{
	keyboard = MainApp::instance()->getKeyboard();
	GUIMgr = GUIManager::instance();
	initTimer = 1.0;
}

void ShipSelection::handleStates()
{
	if (state >= shipsNum) {
		state = 0;
	}
	else if (state < 0) {
		state = shipsNum - 1;
	}
	//Rota hasta que llega a la maxRotation que es la posición de reposo
	if (rotate!=0 && currentRotation < maxRotation) {
		circlePivot->yaw(rotate*0.15);
		currentRotation += 0.15;
	}
	else {
		rotate = 0;
		currentRotation = 0;
	}
}

ShipSelection::~ShipSelection()
{
}

void ShipSelection::Update(float t)
{
	if (currentTime > initTimer) {
		//Timer que regula la velocidad de input
		if (lastKey == OIS::KC_A || lastKey == OIS::KC_D) {
			lastTimePressed += t;
			if (lastTimePressed > inputBufferTimer) {
				lastKey = OIS::KC_U;
				lastTimePressed = 0;
			}
		}
		//Cuando pulsas A baja el estado y rota hacia la izquierda
		if (keyboard->isKeyDown(OIS::KC_A) && lastKey != OIS::KC_A && rotate == 0) {
			state--;
			currentRotation = 0;
			rotate = 1;
			lastKey = OIS::KC_A;
		}
		else if (keyboard->isKeyDown(OIS::KC_D) && lastKey != OIS::KC_D && rotate == 0) {
			//Con D aumenta el estado y rota a la derecha
			state++;
			currentRotation = 0;
			rotate = -1;
			lastKey = OIS::KC_D;
		}
		if (keyboard->isKeyDown(OIS::KC_SPACE) || keyboard->isKeyDown(OIS::KC_INSERT)) {
			MainApp::instance()->getCurrentScene()->hideGUI();
			selectShip();
		}
		handleStates();
		shipsAnimation();
	}
	else {
		currentTime += MainApp::instance()->deltaTime();
	}
}

void ShipSelection::reciveMsg(Message * msg)
{
}
