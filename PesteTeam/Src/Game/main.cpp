#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <GameObjectFactory.h>

int main() {

	MainApp::instance()->initApp();
	GameObjectFactory* fact = new GameObjectFactory("./Assets/Scenes/");
	fact->loadPrefabs();
	fact->loadScene("Scene1");
	Scene* escena1 = new Scene();
	MainApp::instance()->AddScene(escena1);
	MainApp::instance()->runGame();
	
	return 0;
}