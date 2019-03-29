#include "json.hpp"
#include <string>
#include <map>
#include <vector>
#include <list>
#include "BehaviourComponent.h"
#include "GameObject.h"
#include "Scene.h"
#ifndef _SCENELOADER_H
#define _SCENELOADER_H
using json = nlohmann::json;
class SceneLoader
{
private:
	//Diccionario con el contenido de los prefabs  key=prefab type  value=prefab parameters
	//Con este diccionario no va a ser necesario abrir el archivo prefabs cada vez que queramos cargar uno
	std::map<std::string, std::string> prefabsMap;
	std::string scenesPath;
	std::map<std::string, Scene*> scenesMap;
public:
	SceneLoader(std::string scenesPath);
	bool loadPrefabsFromFile();
	bool loadSceneFromFile(std::string sceneName);

	bool loadTestScene();

	GameObject* createGameObject(json gameObject_json, std::vector<float> position, Scene* scene);
	void addComponents(json components_json, GameObject* go, Scene* scene);
	Scene* getScene(std::string sceneName);
	~SceneLoader();
};

#endif