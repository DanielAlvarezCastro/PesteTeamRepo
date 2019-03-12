#include <iostream>
#include <MainApp.h>
#include <Scene.h>
int main() {
	
	MainApp Application;

	Application.initApp();
	Scene* escena1 = new Scene(&Application);
	Application.AddScene(escena1);
	Application.runGame();
	return 0;
}