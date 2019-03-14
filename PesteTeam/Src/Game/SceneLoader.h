#include "json.hpp"
#include <string>
#include <map>
#include <vector>
#include <list>
#include "BehaviourComponent.h"
#include "GameObject.h"
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
public:
	SceneLoader(std::string scenesPath);
	bool loadPrefabs();
	bool loadScene(std::string sceneName);
	GameObject* createGameObject(json gameObject_json, std::vector<float> position);
	void addComponents(json components_json, GameObject* go);
	~SceneLoader();
};

#endif