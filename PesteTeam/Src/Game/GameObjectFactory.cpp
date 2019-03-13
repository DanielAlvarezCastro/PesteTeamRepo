#include "GameObjectFactory.h"
#include <fstream>
#include <iostream>
using json = nlohmann::json;

GameObjectFactory::GameObjectFactory(std::string scenesPath) : scenesPath(scenesPath)
{
}

bool GameObjectFactory::loadPrefabs()
{
	std::cout << "Cargando prefabs..." << std::endl;
	//Carga el json de los prefabs
	std::string prefabsFilename = scenesPath + "Prefabs.json";
	std::ifstream prefabsFile;
	prefabsFile.open(prefabsFilename.c_str());
	if (prefabsFile.fail())
	{
		std::cerr << "Los prefabs no se pudieron cargar " <<std::endl;
		return false;
	}
	json prefabs_json;
	prefabsFile >> prefabs_json;

	//Obtiene el valor del "tipo" de prefab y lo utiliza como key para el diccionario
	//El valor se queda como un string en formato json
	for (json::iterator it = prefabs_json["Prefabs"].begin(); it != prefabs_json["Prefabs"].end(); ++it) {
		std::string key = (*it)["Type"].get<std::string>();
		std::string value = (*it).dump();
		prefabsMap.insert(std::pair<std::string, std::string>(key, value));
	}

	prefabsFile.close();
	std::cout << "Prefabs cargados con exito!" << std::endl;
	return true;
}



bool GameObjectFactory::loadScene(std::string sceneName)
{
	std::cout << "********Cargando " << sceneName << "********" << std::endl;
	//Carga el archivo que contiene la escena
	std::string sceneFilename;
	sceneFilename = scenesPath + sceneName + ".json";
	std::ifstream sceneFile;
	sceneFile.open(sceneFilename.c_str());
	if (sceneFile.fail())
	{
		std::cerr << "No se ha podido cargar " << sceneFilename << std::endl;
		return false;
	}
	json scene_json;
	sceneFile >> scene_json;

	//Itera sobre los gameobjects en el json de la escena
	for (json::iterator it = scene_json["GameObjects"].begin(); it != scene_json["GameObjects"].end(); ++it) {

		//Guarda el valor de su posición y de su tipo
		std::string prefabType = (*it)["Type"].get<std::string>();
		std::vector<float> position = (*it)["Position"].get<std::vector<float>>();
		//Si el tipo de prefab existe entonces crea el gameobject a partir del diccionario
		if (prefabsMap.find(prefabType)!=prefabsMap.end()) {
			json prefab_json = json::parse(prefabsMap[(*it)["Type"]]);
			GameObject* go = createGameObject(prefab_json, position);
			addComponents(prefab_json["Components"], go);
		}
		else {
			//Si no existe entonces lo crea leyendo el json de la escena
			GameObject* go = createGameObject((*it), position);
			addComponents((*it)["Components"], go);
		}
	}

	//Cierra el archivo de la escena
	sceneFile.close();
	std::cout << sceneName << " cargado con exito!" << std::endl;
	return true;
}

GameObject* GameObjectFactory::createGameObject(json gameObject_json, std::vector<float> position)
{
	GameObject* ob = new GameObject();
	std::string name= gameObject_json["Name"].get<std::string>();
	std::string meshName = gameObject_json["Mesh"].get<std::string>();
	//Añadir a la escena de ogre 
	return ob;
}

void GameObjectFactory::addComponents(json components_json, GameObject * go)
{
	for (json::iterator itComponent = components_json.begin(); itComponent != components_json.end(); ++itComponent) {
		std::string componentName = (*itComponent)["Name"].get<std::string>();
		if (componentName == "HealthScript") {
			float health = (*itComponent)["Health"].get<float>();
			float time = (*itComponent)["Time"].get<float>();
			//Hacer new Healthscript() y añadirselo al go
		}
		else if (componentName == "Shooter") {

		}
	}	
}


GameObjectFactory::~GameObjectFactory()
{
}
