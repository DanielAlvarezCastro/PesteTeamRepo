#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <SceneLoader.h>

int main() {

	MainApp::instance()->initApp();
	SceneLoader* loader = new SceneLoader("./Assets/Scenes/");
	if(!loader->loadPrefabs())
		return false;
	if(!loader->loadScene("Scene1"))
		return false;
	Scene* escena1 = new Scene();
	MainApp::instance()->AddScene(escena1);
	MainApp::instance()->runGame();
	
	return 0;
}