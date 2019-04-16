#pragma once
#include <Ogre.h>
#include "GameObject.h"
#include <SoundManager.h>
#include <MyGUI.h>
class Scene
{
protected:
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	Ogre::Camera* mCamera;

	std::vector<GameObject*> gameObjects;
	std::vector<BasicComponent*> components;
	std::vector<MyGUI::Widget*> guiObjects;
	
	std::string name;
public:
	Scene();
	~Scene();
	void createScene(String sceneName);
	void updateScene();

	std::string getName() { return name; };

	void hideGUI(){
		for (int i = 0; i < guiObjects.size(); i++) {
			guiObjects[i]->setVisible(false);
		}
	}
	void showGUI() {
		for (int i = 0; i < guiObjects.size(); i++) {
			guiObjects[i]->setVisible(true);
		}
	}

	void addGUIObject(MyGUI::Widget* ob) { guiObjects.push_back(ob); }
	void addGameObject(GameObject* GO) { gameObjects.push_back(GO); };
	void addComponent(BasicComponent* bc) { components.push_back(bc); };
	void addCamera(Ogre::Camera* cam) { mCamera = cam; };
	GameObject* getGameObject(std::string name) {
		for (int i = 0; i < gameObjects.size(); i++) {
			if (gameObjects[i]->getName() == name) {
				return gameObjects[i];
			}
	} }
	Ogre::Camera* getCamera() {	return mCamera;	};
	Ogre::SceneManager* getSceneManager() { return mSceneMgr; };
	void reciveMsg(Message* msg) { for (BasicComponent* cp : components) cp->reciveMsg(msg); };
};

