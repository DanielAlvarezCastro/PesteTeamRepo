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
	void setPlayerMesh(string meshName);
	bool LoadGame();
	bool LoadScene(string sceneName);
	void CloseScene();
	static GameSceneManager* instance() {
		if (instance_ == nullptr)
			instance_ = new GameSceneManager();
		return instance_;
	}
};

