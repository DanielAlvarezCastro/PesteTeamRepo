#pragma once
#include <string>
#include <SceneLoader.h>
using namespace std;
class GameSceneManager
{
private:
	static GameSceneManager* instance_;
	SceneLoader* loader;
public:
	GameSceneManager();
	~GameSceneManager();
	void setPlayerShip(string shipName);
	bool LoadGame();
	bool LoadScene(string sceneName);
	void CloseScene(string nextScene ="");
	static GameSceneManager* instance() {
		if (instance_ == nullptr)
			instance_ = new GameSceneManager();
		return instance_;
	}
};

