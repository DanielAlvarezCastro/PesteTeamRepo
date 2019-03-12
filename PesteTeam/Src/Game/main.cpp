#include <iostream>
#include <MainApp.h>
#include <Scene.h>

int main() {

	MainApp::instance()->initApp();
	Scene* escena1 = new Scene();
	MainApp::instance()->AddScene(escena1);
	MainApp::instance()->runGame();

	return 0;
}