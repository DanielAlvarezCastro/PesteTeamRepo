#include "SceneLoader.h"
#include <fstream>
#include <iostream>
#include "PlayerController.h"
#include "CameraMovement.h"
#include <vector>
using json = nlohmann::json;

SceneLoader::SceneLoader(std::string scenesPath) : scenesPath(scenesPath)
{
}

bool SceneLoader::loadPrefabsFromFile()
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



bool SceneLoader::loadSceneFromFile(std::string sceneName)
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

	Scene* escena = new Scene();
	escena->createScene("primary");
	scenesMap.insert(pair<std::string, Scene*>(sceneName, escena));

	//Itera sobre los gameobjects en el json de la escena
	for (json::iterator it = scene_json["GameObjects"].begin(); it != scene_json["GameObjects"].end(); ++it) {

		//Guarda el valor de su posición y de su tipo
		std::string prefabType = (*it)["Type"].get<std::string>();
		std::vector<float> position = (*it)["Position"].get<std::vector<float>>();
		//Si el tipo de prefab existe entonces crea el gameobject a partir del diccionario
		if (prefabsMap.find(prefabType)!=prefabsMap.end()) {
			json prefab_json = json::parse(prefabsMap[(*it)["Type"]]);
			GameObject* go = createGameObject(prefab_json, position, escena);
			escena->addGameObject(go);
			addComponents(prefab_json["Components"], go, escena);
		}
		else {
			//Si no existe entonces lo crea leyendo el json de la escena
			GameObject* go= createGameObject((*it), position, escena);
			addComponents((*it)["Components"], go, escena);
		}
	}

	//Cierra el archivo de la escena
	sceneFile.close();
	std::cout << sceneName << " cargado con exito!" << std::endl;
	return true;
}

//Escenas de prueba para meterlas desde código aquí
bool SceneLoader::loadTestScene()
{
	Scene* escena1 = new Scene();
	escena1->createScene("primary");

	GameObject* Nave = new GameObject();
	Nave->createEntity("SkyGrasper.mesh", "Player", escena1);
	Nave->setScale(Vec3(2, 2, 2));
	Nave->setPosition(Vec3(0, -7, 35));

	GameObject* cubito = new GameObject();
	cubito->createEntity("cube.mesh", "Cubito", escena1);
	cubito->setScale(Vec3(0.1, 0.1, 0.1));
	cubito->setPosition(Vec3(0, 10, 5));
	RigidBody* rb = new RigidBody(cubito, "Cubito", 5.0);
	escena1->addComponent(rb);

	GameObject* cubito2 = new GameObject();
	cubito2->createEntity("cube.mesh", "Cubito2", escena1);
	cubito2->setScale(Vec3(0.2, 0.2, 0.2));
	cubito2->setPosition(Vec3(0, -30, 5));
	RigidBody* rb2 = new RigidBody(cubito2, "Cubito2");
	escena1->addComponent(rb2);

	GameObject* pointer = new GameObject();
	pointer->createEntity("cube.mesh", "Pointer", escena1);
	pointer->setScale(Vec3(0.05, 0.05, 0.05));
	pointer->setPosition(Vec3(0, -7, 5));

	escena1->addGameObject(Nave);
	escena1->addGameObject(pointer);
	escena1->addGameObject(cubito);

	Ogre::Camera* mCamera = escena1->getSceneManager()->createCamera("MainCam");
	mCamera->setNearClipDistance(5);
	escena1->addCamera(mCamera);
	GameObject* cameraOb = new GameObject();
	cameraOb->createEmptyEntity("MainCam", escena1);
	cameraOb->attachCamera(mCamera);
	cameraOb->setPosition(Vec3(0, 0, 80));
	cameraOb->lookAt(Vec3(0, 0, -300), Ogre::Node::TS_WORLD);

	escena1->addGameObject(cameraOb);

	Ogre::Light* luz = escena1->getSceneManager()->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(.75, .75, .75);
	

	escena1->getSceneManager()->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	GameObject* l1Ob = new GameObject();
	l1Ob->createEmptyEntity("mLight", escena1);
	l1Ob->attachLight(luz);
	escena1->addGameObject(l1Ob);

	PlayerController* pc = new PlayerController(Nave, pointer);
	escena1->addComponent(pc);

	CameraMovement* cM = new CameraMovement(cameraOb, Nave);
	escena1->addComponent(cM);


	MeshManager::getSingleton().createPlane("mPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Ogre::Vector3::UNIT_Y, 0), 12000, 12000, 1, 1, true, 1, 1.0, 1.0, Ogre::Vector3::UNIT_Z);

	GameObject* planeOb = new GameObject();
	planeOb->createEntity("mPlane", "Floor", escena1);
	planeOb->setMaterial("Test/FloorTile");
	planeOb->setPosition(Vec3(0, -60, 0));
	escena1->addGameObject(planeOb);

	GameObject* planeOb2 = new GameObject();
	planeOb2->createEntity("mPlane", "Floor2", escena1);
	planeOb2->setMaterial("Test/FloorTileLight");
	planeOb2->setPosition(Vec3(0, -59.9, 0));
	escena1->addGameObject(planeOb2);

	
	
	

	scenesMap.insert(pair<std::string, Scene*>("Scene1", escena1));


	Scene* escena2 = new Scene();
	escena2->createScene("secondary");
	GameObject* cameraOb2 = new GameObject();
	Ogre::Camera* mCamera2 = escena2->getSceneManager()->createCamera("MainCam2");
	mCamera2->setNearClipDistance(5);
	cameraOb2->createEmptyEntity("MainCam2", escena2);
	cameraOb2->attachCamera(mCamera2);
	cameraOb2->setPosition(Vec3(0, 0, 80));
	cameraOb2->lookAt(Vec3(0, 0, -300), Ogre::Node::TS_WORLD);

	//GameObject* cubito = new GameObject();
	//cubito->createEntity("cube.mesh", "Cubito", escena2);
	//cubito->setScale(Vec3(0.2, 0.2, 0.2));
	//cubito->setPosition(Vec3(0, -7, 5));
	//escena2->addGameObject(cubito);
	escena2->addCamera(mCamera2);

	escena2->addGameObject(cameraOb2);


	Ogre::Light* luz2 = escena2->getSceneManager()->createLight("Luz2");
	luz2->setType(Ogre::Light::LT_DIRECTIONAL);
	luz2->setDiffuseColour(0.75, 0.75, 0.75);

	GameObject* l2Ob = new GameObject();
	l2Ob->createEmptyEntity("mLight2", escena2);
	l2Ob->attachLight(luz2);

	escena2->addGameObject(l2Ob);

	scenesMap.insert(pair<std::string, Scene*>("Scene2", escena2));

	return true;
}

GameObject* SceneLoader::createGameObject(json gameObject_json, std::vector<float> position, Scene* scene)
{
	GameObject* ob = new GameObject();
	std::string name= gameObject_json["Name"].get<std::string>();
	std::string meshName = gameObject_json["Mesh"].get<std::string>();
	if (meshName == "None") {
		ob->createEmptyEntity(name, scene);
	}
	else ob->createEntity(meshName, name, scene);

	Vec3 pos(position[0], position[1], position[2]);
	ob->setPosition(pos);

	if (gameObject_json.find("Scale") != gameObject_json.end()) {
		std::vector<float> scale = gameObject_json["Scale"].get<std::vector<float>>();
		Vec3 sc(scale[0], scale[1], scale[2]);
		ob->setScale(sc);
	}

	return ob;
}

void SceneLoader::addComponents(json components_json, GameObject * go, Scene* scene)
{
	for (json::iterator itComponent = components_json.begin(); itComponent != components_json.end(); ++itComponent) {
		std::string componentName = (*itComponent)["Name"].get<std::string>();
		if (componentName == "HealthScript") {
			float health = (*itComponent)["Health"].get<float>();
			float time = (*itComponent)["Time"].get<float>();
			//Hacer new Healthscript() y añadirselo al go
		}
		else if (componentName == "Camera") {
			Ogre::Camera* cam = scene->getSceneManager()->createCamera("mCamera");
			int dist = (*itComponent)["NearClipDistance"].get<int>();
			cam->setNearClipDistance(dist);			
			go->attachCamera(cam);
			scene->addCamera(cam);
		}
		else if (componentName == "CameraMovement") {
			std::string pName = (*itComponent)["GameObject"];
			GameObject* target = scene->getGameObject(pName);
			CameraMovement* cM = new CameraMovement(go, target);
			scene->addComponent(cM);
		}
		else if (componentName == "Light") {
			Ogre::Light* luz = scene->getSceneManager()->createLight("Luz");
			if ((*itComponent)["Type"]=="Directional") {
				luz->setType(Ogre::Light::LT_DIRECTIONAL);
			}
			luz->setDiffuseColour(0.75, 0.75, 0.75);
			go->attachLight(luz);
		}
		else if (componentName == "PlayerController") {
			std::string pName = (*itComponent)["GameObject"];
			GameObject* pointer = scene->getGameObject(pName);
			PlayerController* pc = new PlayerController(go, pointer);
			scene->addComponent(pc);
		}
	}	
}

Scene* SceneLoader::getScene(std::string sceneName)
{
	Scene* escena = scenesMap[sceneName];
	return escena;
}


SceneLoader::~SceneLoader()
{
}
