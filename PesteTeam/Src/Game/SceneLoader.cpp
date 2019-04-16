#include "SceneLoader.h"
#include <fstream>
#include <iostream>
#include "PlayerController.h"
#include "CameraMovement.h"
#include "ShipController.h"
#include "MainMenuManager.h"
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



bool SceneLoader::loadSceneFromFile(std::string sceneName, Scene* scene)
{
	scene->createScene(sceneName);
	if (sceneName == "TestScene") {
		loadTestScene(scene);
		return true;
	}
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

	scenesMap.insert(pair<std::string, Scene*>(sceneName, scene));

	//Primero carga la cámara para evitar conflictos con la inicialización de MyGUI
	for (json::iterator it = scene_json["Camera"].begin(); it != scene_json["Camera"].end(); ++it) {
		std::vector<float> position = (*it)["Position"].get<std::vector<float>>();
		GameObject* go = createGameObject((*it), position, scene);
		addComponents((*it)["Components"], go, scene);
	}
	//Set del viewport
	MainApp::instance()->setupViewport(scene->getCamera());

	GUIManager::instance()->initScene(scene);
	//Itera sobre los elementos del GUI
	for (json::iterator it = scene_json["GUI"].begin(); it != scene_json["GUI"].end(); ++it) {
		createGUIObject(*it, scene);
	}


	//Itera sobre los gameobjects en el json de la escena
	for (json::iterator it = scene_json["GameObjects"].begin(); it != scene_json["GameObjects"].end(); ++it) {

		//Guarda el valor de su posici�n y de su tipo
		std::string prefabType = (*it)["Type"].get<std::string>();
		std::vector<float> position = (*it)["Position"].get<std::vector<float>>();
		//Si el tipo de prefab existe entonces crea el gameobject a partir del diccionario
		if (prefabsMap.find(prefabType) != prefabsMap.end()) {
			json prefab_json = json::parse(prefabsMap[(*it)["Type"]]);
			GameObject* go = createGameObject(prefab_json, position, scene);
			scene->addGameObject(go);
			addComponents(prefab_json["Components"], go, scene);
		}
		else {
			//Si no existe entonces lo crea leyendo el json de la escena
			GameObject* go = createGameObject((*it), position, scene);
			addComponents((*it)["Components"], go, scene);
		}
	}

	//Cierra el archivo de la escena
	sceneFile.close();
	std::cout << sceneName << " cargado con exito!" << std::endl;
	return true;
}

Scene* SceneLoader::loadSceneFromMemory(std::string sceneName, Scene * scene)
{
	scene = scenesMap[sceneName];
	MainApp::instance()->setupViewport(scene->getCamera());
	GUIManager::instance()->initScene(scene);
	return scene;
}

bool SceneLoader::sceneAlreadyLoaded(std::string sceneName)
{

	//Si el nombre de la escena existe eso quiere decir que ya está cargada
	if (scenesMap.find(sceneName) != scenesMap.end()) {
		//scene = scenesMap[sceneName];
		return true;
	}
	else return false;
}

//Escenas de prueba para meterlas desde c�digo aqu�
bool SceneLoader::loadTestScene(Scene* scene)
{
	GameObject* pointer = new GameObject();
	pointer->createEmptyEntity("Pointer", scene);
	pointer->setPosition(Vec3(0, 40, 0));
	pointer->setDirection(Vec3(0, 0, -1));
	scene->addGameObject(pointer);

	GameObject* Nave = new GameObject();
	Nave->createEntity("SXR-72.mesh", "Player", scene);
	Nave->setScale(Vec3(3, 3, 3));
	Nave->asingFather(pointer);
	Nave->setPosition(Vec3(-1, 0, 0));
	scene->addGameObject(Nave);

	GameObject* pivot = new GameObject();
	pivot->createEmptyEntity("Pivot", scene);
	pivot->setScale(Vec3(0.02, 0.02, 0.02));
	pivot->asingFather(pointer);
	pivot->setPosition(Vec3(0, 0, -50));
	scene->addGameObject(pivot);

	GameObject* cubito = new GameObject();
	cubito->createEntity("cube.mesh", "Cubito", scene);
	cubito->setScale(Vec3(0.1, 0.1, 0.1));
	cubito->setPosition(Vec3(10, 40, -15));
	RigidBody* rb = new RigidBody(cubito, "Cubito", 5.0);
	cubito->addRigidbody(rb);
	scene->addComponent(rb);

	GameObject* edificio1 = new GameObject();
	edificio1->createEntity("Building1.mesh", "Edificio1", scene);
	edificio1->setScale(Vec3(5, 5, 5));
	edificio1->setPosition(Vec3(0, 0, -300));

	GameObject* edificio2 = new GameObject();
	edificio2->createEntity("Building2.mesh", "Edificio2", scene);
	edificio2->setScale(Vec3(5, 5, 5));
	edificio2->setPosition(Vec3(100, 0, -100));

	GameObject* cubito2 = new GameObject();
	cubito2->createEntity("cube.mesh", "Cubito2", scene);
	cubito2->setScale(Vec3(0.2, 0.2, 0.2));
	cubito2->setPosition(Vec3(0, 10, -15));
	RigidBody* rb2 = new RigidBody(cubito2, "Cubito2");
	cubito2->addRigidbody(rb2);
	scene->addComponent(rb2);


	scene->addGameObject(Nave);
	scene->addGameObject(edificio1);
	scene->addGameObject(pointer);
	scene->addGameObject(cubito);

	Ogre::Camera* mCamera = scene->getSceneManager()->createCamera("MainCam");
	mCamera->setNearClipDistance(5);
	scene->addCamera(mCamera);
	GameObject* cameraOb = new GameObject();
	cameraOb->createEmptyEntity("MainCam", scene);
	cameraOb->attachCamera(mCamera);
	cameraOb->asingFather(pointer);
	cameraOb->setPosition(Vec3(0, 0, 40));

	scene->addGameObject(cameraOb);

	//Set del viewport
	MainApp::instance()->setupViewport(scene->getCamera());

	Ogre::Light* luz = scene->getSceneManager()->createLight("Luz");
	luz->setType(Ogre::Light::LT_DIRECTIONAL);
	luz->setDiffuseColour(.75, .75, .75);
	

	scene->getSceneManager()->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	GameObject* l1Ob = new GameObject();
	l1Ob->createEmptyEntity("mLight", scene);
	l1Ob->attachLight(luz);
	scene->addGameObject(l1Ob);

	PlayerController* pc = new PlayerController(pointer);
	scene->addComponent(pc);

	ShipController* sc = new ShipController(Nave);
	scene->addComponent(sc);

	CameraMovement* cM = new CameraMovement(cameraOb, pointer, pivot);
	scene->addComponent(cM);
	   
	GameObject* planeOb = new GameObject();
	planeOb->createEntity("FloorGrid.mesh", "Floor", scene);
	//planeOb->setMaterial("Test/FloorTile");
	planeOb->setScale(Vec3(100, 100, 100));
	planeOb->setPosition(Vec3(0, 0,-100));
	scene->addGameObject(planeOb);


	GameObject* mountains = new GameObject();
	mountains->createEntity("Mountains.mesh", "Mountains", scene);
	//planeOb->setMaterial("Test/FloorTile");
	mountains->setScale(Vec3(100, 100, 100));
	mountains->setPosition(Vec3(-2500, 350, -100));
	scene->addGameObject(mountains);
	Ogre::ColourValue fadeColour(0.0, 0.0, 0.0);
	scene->getSceneManager()->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);

	GUIManager::instance()->initScene(scene);

	MyGUI::ImageBox* b = GUIManager::instance()->createImage("CircleLogo.png", 0, 0, 200, 200, "ImageBox", "Circulo.png");
	scene->addGUIObject(b);
	
	

	scenesMap.insert(pair<std::string, Scene*>("TestScene", scene));


	return true;
}

void SceneLoader::deleteScene(std::string sceneName)
{
	scenesMap.erase(sceneName);
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

void SceneLoader::createGUIObject(json gui_json, Scene* scene)
{
	//Common gui attributes
	std::string guiName = gui_json["Name"].get<std::string>();
	std::string guiType = gui_json["Type"].get<std::string>();
	std::vector<float> position = gui_json["Position"].get<std::vector<float>>();
	std::vector<float> size = gui_json["Size"].get<std::vector<float>>();
	if (guiType == "ImageBox") {
		std::string source = gui_json["Src"].get<std::string>();
		MyGUI::ImageBox* b = GUIManager::instance()->createImage(source, position[0], position[1], size[0], size[1], guiType, guiName);
		scene->addGUIObject(b);
	}
	else if (guiType == "TextBox") {
		std::string caption = gui_json["Caption"].get<std::string>();
		int fontHeight = gui_json["FontHeight"].get<int>();
		GUIManager::instance()->createTextbox(caption, position[0], position[1], size[0], size[1], guiType, guiName, fontHeight);
	}
}

void SceneLoader::addComponents(json components_json, GameObject * go, Scene* scene)
{
	for (json::iterator itComponent = components_json.begin(); itComponent != components_json.end(); ++itComponent) {
		std::string componentName = (*itComponent)["Name"].get<std::string>();
		if (componentName == "HealthScript") {
			float health = (*itComponent)["Health"].get<float>();
			float time = (*itComponent)["Time"].get<float>();
			//Hacer new Healthscript() y a�adirselo al go
		}
		else if (componentName == "Camera") {
			std::string cameraName = (*itComponent)["CameraName"].get<std::string>();
			Ogre::Camera* cam = scene->getSceneManager()->createCamera(cameraName);
			int dist = (*itComponent)["NearClipDistance"].get<int>();
			cam->setNearClipDistance(dist);			
			go->attachCamera(cam);
			scene->addCamera(cam);
		}
		else if (componentName == "CameraMovement") {
			std::string pName = (*itComponent)["GameObject"];
			GameObject* target = scene->getGameObject(pName);
			GameObject* pivot = scene->getGameObject(pName);
			CameraMovement* cM = new CameraMovement(go, target, pivot);
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
			PlayerController* pc = new PlayerController(pointer);
			scene->addComponent(pc);
		}
		else if (componentName == "MainMenuManager") {
			int titleAmp = (*itComponent)["TitleAmplitude"];
			float titleSinPeriod = (*itComponent)["TitleSinPeriod"];
			int buttonAmp = (*itComponent)["ButtonAmplitude"];
			float buttonSinPeriod = (*itComponent)["ButtonSinPeriod"];
			MainMenuManager* MMM = new MainMenuManager(go);
			MMM->setTitleAmplitude(titleAmp);
			MMM->setTitleSinPeriod(titleSinPeriod);
			MMM->setButtonAmplitude(buttonAmp);
			MMM->setButtonSinPeriod(buttonSinPeriod);
			scene->addComponent(MMM);
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
