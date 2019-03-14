#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <SceneLoader.h>
#include "PlayerController.h"

int main() {

	MainApp::instance()->initApp();
	SceneLoader* loader = new SceneLoader("./Assets/Scenes/");
	if(!loader->loadPrefabs())
		return false;
	if(!loader->loadScene("Scene1"))
		return false;
	Scene* escena1 = new Scene();
	MainApp::instance()->AddScene(escena1);
	GameObject* Nave = new GameObject();
	Nave->createEntity("SkyGrasper.mesh");
	Nave->setScale(Vec3(2, 2, 2));
	Nave->setPosition(Vec3(0, -7, 35));
	escena1->addGameObject(Nave);
	PlayerController* pc = new PlayerController(Nave);
	escena1->addComponent(pc);
	MainApp::instance()->runGame();
	
	return 0;
}