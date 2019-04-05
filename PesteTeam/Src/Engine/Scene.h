#pragma once
#include <Ogre.h>
#include "GameObject.h"
#include <SoundManager.h>

class Scene
{
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera* mCamera;

	std::vector<GameObject*> gameObjects;
	std::vector<BasicComponent*> components;

	ISound* intro_;
	ISound* body_;

	bool playing_music_ = false;

public:
	Scene();
	~Scene();
	void createScene(String sceneName);
	void updateScene();

	void addGameObject(GameObject* GO) { gameObjects.push_back(GO); };
	void addComponent(BasicComponent* GO) { components.push_back(GO); };
	void addCamera(Ogre::Camera* cam) { mCamera = cam; };
	GameObject* getGameObject(std::string name) {
		for (int i = 0; i < gameObjects.size(); i++) {
			if (gameObjects[i]->getName() == name) {
				return gameObjects[i];
			}
	} }
	Ogre::Camera* getCamera() {	return mCamera;	};
	Ogre::SceneManager* getSceneManager() { return mSceneMgr; };
};

