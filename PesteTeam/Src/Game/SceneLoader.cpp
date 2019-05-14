#include "SceneLoader.h"
#include <fstream>
#include <iostream>
#include "PlayerController.h"
#include "CameraMovement.h"
#include "ShipController.h"
#include "MainMenuManager.h"
#include <vector>
#include "ShipSelection.h"
#include "TurretBehaviour.h"
#include "ShotBehaviour.h"
#include "FlyerBehaviour.h"
#include "TargetController.h"
#include "EnemyBehaviour.h"
#include "EnemyManager.h"
#include "CreditsManager.h"
#include "GameGUI.h"
#include "GameManager.h"
#include "PauseMenuManager.h"

#include "EnemyShoot.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
using json = nlohmann::json;

SceneLoader::SceneLoader(std::string scenesPath) : scenesPath(scenesPath)
{
	shipStats.push_back(3);
	shipStats.push_back(3);
	shipStats.push_back(3);
}

void OnCuboCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints) {
	//std::cout << "Soy un cubo y he chocado" << std::endl;
}

void OnCubo2Collision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints) {
	//std::cout << "Juan wapo" << std::endl;
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

	//Se cargan las opciones de la escena
	//Primero carga la cámara para evitar conflictos con la inicialización de MyGUI
	std::string cameraName = scene_json["Camera"].get<std::string>();
	Ogre::Camera* cam = scene->getSceneManager()->createCamera(cameraName);
	scene->addCamera(cam);
	
	//Set del viewport
	MainApp::instance()->setupViewport(scene->getCamera());
	MainApp::instance()->addCompositor("Interference");

	GUIManager::instance()->initScene(scene);
	//Itera sobre los elementos del GUI
	for (json::iterator it = scene_json["GUI"].begin(); it != scene_json["GUI"].end(); ++it) {
		createGUIObject(*it, scene);
	}


	//Itera sobre los gameobjects en el json de la escena
	for (json::iterator it = scene_json["GameObjects"].begin(); it != scene_json["GameObjects"].end(); ++it) {

		//Guarda el valor de su posici�n y de su tipo
		std::string prefabType = (*it)["Type"].get<std::string>();
		//Si el tipo de prefab existe entonces crea el gameobject a partir del diccionario
		if (prefabsMap.find(prefabType) != prefabsMap.end()) {
			json prefab_json = json::parse(prefabsMap[(*it)["Type"]]);
			GameObject* go = createGameObjectFromPrefab(prefab_json, (*it),  scene);
			scene->addGameObject(go);		

		}
		else {
			std::vector<float> position = (*it)["Position"].get<std::vector<float>>();
			//Si no existe entonces lo crea leyendo el json de la escena
			GameObject* go = createGameObject((*it), scene);
			scene->addGameObject(go);
		}
		
	}

	//Cierra el archivo de la escena
	sceneFile.close();
	std::cout << sceneName << " cargado con exito!" << std::endl;
	return true;
}

Scene* SceneLoader::loadSceneFromMemory(std::string sceneName)
{
	Scene* scene = scenesMap[sceneName];
	MainApp::instance()->setupViewport(scene->getCamera());
	GUIManager::instance()->initScene(scene);
	cout << sceneName << "cargado" << endl;
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
	Physics::getInstance()->initDebuger(scene->getSceneManager());
	//Physics::getInstance()->setDebugState(true);

	GameObject* pointer = new GameObject();
	pointer->createEmptyEntity("Pointer", scene);
	pointer->setPosition(Vec3(0, 40, 0));
	pointer->setDirection(Vec3(0, 0, -1));
	scene->addGameObject(pointer);

	GameObject* Nave = new GameObject();
	Nave->createEntity(playerShip + ".mesh", "Player", scene);
	Nave->setScale(Vec3(3, 3, 3));
	
	Nave->asingFather(pointer);
	Nave->setPosition(Vec3(-1, 0, 0));
	scene->addGameObject(Nave);
	RigidBody* rbNave = new RigidBody(Nave, "Nave", 5, true);
	scene->addComponent(rbNave);

	GameObject* nearTarget = new GameObject();
	nearTarget->createEmptyEntity("nearTarget", scene);
	nearTarget->asingFather(pointer);
	nearTarget->setPosition(Vec3(0, 0, -100));
	scene->addGameObject(nearTarget);

	GameObject* farTarget = new GameObject();
	farTarget->createEmptyEntity("farTarget", scene);
	farTarget->asingFather(pointer);
	farTarget->setPosition(Vec3(0, 0, -200));
	scene->addGameObject(farTarget);


	GameObject* pivot = new GameObject();
	pivot->createEmptyEntity("Pivot", scene);
	pivot->setScale(Vec3(0.02, 0.02, 0.02));
	pivot->asingFather(pointer);
	pivot->setPosition(Vec3(0, 0, -50));
	scene->addGameObject(pivot);

	GameObject* pivot1 = new GameObject();
	pivot1->createEmptyEntity("Pivot1", scene);
	pivot1->asingFather(pointer);
	pivot1->setPosition(Vec3(7, 0, -15));
	pivot1->setDirection(Vec3(0, 0, -1));
	scene->addGameObject(pivot1);

	GameObject* pivot2 = new GameObject();
	pivot2->createEmptyEntity("Pivot2", scene);
	pivot2->asingFather(pointer);
	pivot2->setPosition(Vec3(-7, 0, -15));
	pivot2->setDirection(Vec3(0, 0, -1));
	scene->addGameObject(pivot2);

	GameObject* cubito = new GameObject();
	cubito->createEntity("cube.mesh", "Cubito", scene);
	cubito->setScale(Vec3(0.5, 0.5, 0.5));
	cubito->setPosition(Vec3(10, 80, -15));
	RigidBody* rb = new RigidBody(cubito, "Cubito", 0.0001);
	rb->setCollisionCallback(OnCuboCollision);
	//cubito->addRigidbody(rb);
	scene->addComponent(rb);

	GameObject* edificio1 = new GameObject();
	edificio1->createEntity("Building1.mesh", "Edificio1", scene);
	edificio1->setScale(Vec3(5, 5, 5));
	edificio1->setPosition(Vec3(-300, 0, -500));

	RigidBody* rbBuild1 = new RigidBody(edificio1, "Edificio1");
	rbBuild1->setOffset(0, edificio1->getBoundingBox().y / 2);
	scene->addComponent(rbBuild1);


	GameObject* turretBase = new GameObject();
	turretBase->createEntity("TurretBase.mesh", "TurretBase", scene);
	turretBase->setScale(Vec3(20, 20, 20));
	turretBase->setPosition(Vec3(0, 0, -400));

	GameObject* turret = new GameObject();
	turret->createEntity("Turret.mesh", "Turret", scene);
	turret->setScale(Vec3(20, 20, 20));
	turret->setPosition(Vec3(0, 40, -400));
	turret->lookAt(Vec3(-1, 0, 0));
	RigidBody* rbt = new RigidBody(turret, "R_Turret", 5, true);
	scene->addComponent(rbt);

	TurretBehaviour* tB = new TurretBehaviour(turret, pointer);
	scene->addComponent(tB);
	EnemyShoot* tBEH = new EnemyShoot(turret, enemyType::groundTurret, pointer, 10,450,  "EnemyBullet.mesh", 1.6);
	scene->addComponent(tBEH);

	GameObject* pivotT1 = new GameObject();
	pivotT1->createEmptyEntity("PivotT1", scene);
	//como pilla la escala del padre la dividimos para dejarla a niveles globales
	pivotT1->setScale(Vec3((1/turret->getScale().x)*0.05,( 1/turret->getScale().y)*0.05, (1/turret->getScale().z)*0.05));
	pivotT1->asingFather(turret);
	pivotT1->setPosition(Vec3(0.45, 0, -2.5));
	scene->addGameObject(pivotT1);

	GameObject* pivotT2 = new GameObject();
	pivotT2->createEmptyEntity("PivotT2", scene);
	pivotT2->setScale(Vec3((1 / turret->getScale().x)*0.05, (1 / turret->getScale().y)*0.05, (1 / turret->getScale().z)*0.05));
	pivotT2->asingFather(turret);
	pivotT2->setPosition(Vec3(-0.45, 0, -2.5));
	scene->addGameObject(pivotT2);



	GameObject* flyer = new GameObject();
	flyer->createEntity("Mogo.mesh", "Flyer1", scene);
	flyer->setScale(Vec3(1.5, 1.5, 1.5));
	flyer->setPosition(Vec3(0, 200, -400));
	RigidBody* rbt2 = new RigidBody(flyer, "R_Flyer", 5, true);
	scene->addComponent(rbt2);

	GameObject* pivotF1 = new GameObject();
	pivotF1->createEmptyEntity("PivotF1", scene);
	pivotF1->setScale(Vec3((1 / flyer->getScale().x*0.25), (1 / flyer->getScale().y*0.25), (1 / flyer->getScale().z*0.25)));
	pivotF1->asingFather(flyer);
	pivotF1->setPosition(Vec3(0, 0, -40));
	scene->addGameObject(pivotF1);

	EnemyShoot* FES = new EnemyShoot(flyer, enemyType::Flyer, pointer, 10, 500, "EnemyBullet.mesh", 2.4);
	scene->addComponent(FES);

	TurretBehaviour* tb2 = new TurretBehaviour(flyer, pointer);
	scene->addComponent(tb2);

	FlyerBehaviour* fb1 = new FlyerBehaviour(flyer, pointer, FlyerRoute::YCircular, 100, 25);
	scene->addComponent(fb1);

	EnemyBehaviour* eb1 = new EnemyBehaviour(turret, 40);
	scene->addComponent(eb1);

	EnemyBehaviour* eb2 = new EnemyBehaviour(flyer, 40);
	scene->addComponent(eb2);

	GameObject* enemyManager = new GameObject();
	EnemyManager* em = new EnemyManager(enemyManager);
	em->addEnemy(eb1);
	em->addEnemy(eb2);
	scene->addComponent(em);

	scene->addGameObject(enemyManager);
	scene->addGameObject(turret);
	scene->addGameObject(turretBase);
	scene->addGameObject(edificio1);
	scene->addGameObject(cubito);

	scene->addGameObject(flyer);

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

	PlayerController* pc = new PlayerController(pointer,60);
	scene->addComponent(pc);

	ShipController* sc = new ShipController(Nave, 200, playerShip, 1.0, 1400, 1900);
	scene->addComponent(sc);

	ShotBehaviour* sb = new ShotBehaviour(pointer, playerShip, 10, 10, (1.0/7.0), 1.0);
	scene->addComponent(sb);

	CameraMovement* cM = new CameraMovement(cameraOb, pointer, pivot);
	scene->addComponent(cM);
	   
	GameObject* planeOb = new GameObject();
	planeOb->createEntity("FloorGrid.mesh", "Floor", scene);
	//planeOb->setMaterial("Test/FloorTile");
	planeOb->setScale(Vec3(10, 10, 10));
	planeOb->setPosition(Vec3(0, 0,0));
	scene->addGameObject(planeOb);


	GameObject* mountains = new GameObject();
	mountains->createEntity("Mountains.mesh", "Mountains", scene);
	//planeOb->setMaterial("Test/FloorTile");
	mountains->setScale(Vec3(10, 10, 10));
	mountains->setPosition(Vec3(0, -50, 0));
	scene->addGameObject(mountains);

	GUIManager::instance()->initScene(scene);
	
	TargetController* ftc = new TargetController(farTarget, mCamera, "SmallBlueSight.png","SmallRedSight.png","farSight",30, 30);
	TargetController* ntc = new TargetController(nearTarget, mCamera, "BigBlueSight.png", "BigRedSight.png", "nearSight.png", 60, 60);
	scene->addComponent(ftc);
	scene->addComponent(ntc);

	MyGUI::ImageBox* b = GUIManager::instance()->createImage("HealthBarBlue.png", 340, 20, 400, 40, "ImageBox", "HealthBar");
	scene->addGUIObject(b);
	MyGUI::ImageBox* b2 = GUIManager::instance()->createImage("HealthBarFront.png", 340, 20, 400, 40, "ImageBox", "HealthBarFront");
	scene->addGUIObject(b2);



	MyGUI::ImageBox* g = GUIManager::instance()->createImage("GameOver.png", 0, 0, 1080, 720, "ImageBox", "GameOver");
	g->setVisible(false);
	scene->addGUIObject(g);
	MyGUI::ImageBox* g2 = GUIManager::instance()->createImage("MissionAccomplished.png", 0, 0, 1080, 720, "ImageBox", "MissionAccomplished");
	g2->setVisible(false);
	scene->addGUIObject(g2);



	GameObject* guiOB = new GameObject();
	guiOB->createEmptyEntity("guiOb", scene);
	scene->addGameObject(guiOB);

	GameGUI* GGUI = new GameGUI(guiOB, 200);

	scene->addComponent(GGUI);


	GameObject* gameManagerOb = new GameObject();
	gameManagerOb->createEmptyEntity("gameManagerOb", scene);
	scene->addGameObject(gameManagerOb);

	GameManager* GM = new GameManager(gameManagerOb, 3, 5.0);

	scene->addComponent(GM);
	

	scenesMap.insert(pair<std::string, Scene*>("TestScene", scene));


	return true;
}

void SceneLoader::deleteScene(std::string sceneName)
{
	delete scenesMap[sceneName];
	scenesMap.erase(sceneName);
	enemies.clear();
}

void SceneLoader::setPlayerShip(std::string shipName, std::vector<int> _shipStats)
{
	playerShip = shipName;
	shipStats = _shipStats;
}

GameObject* SceneLoader::createGameObject(json gameObject_json, Scene* scene, std::string fatherName)
{
	GameObject* ob = new GameObject();
	std::string name= fatherName + gameObject_json["Name"].get<std::string>();
	std::string meshName = gameObject_json["Mesh"].get<std::string>();
	std::vector<float> position = gameObject_json["Position"].get<std::vector<float>>();
	if (meshName == "SelectedShip") {
		meshName = playerShip + ".mesh";
	}
	//Si no tiene mesh crea una entidad vacía
	if (meshName == "None") {
		ob->createEmptyEntity(name, scene);
	}
	else ob->createEntity(meshName, name, scene);

	Vec3 pos(position[0], position[1], position[2]);
	ob->setPosition(pos);
	//Si tiene el parámetro escala lo asigna
	if (gameObject_json.find("Scale") != gameObject_json.end()) {
		std::vector<float> scale = gameObject_json["Scale"].get<std::vector<float>>();
		Vec3 sc(scale[0], scale[1], scale[2]);
		ob->setScale(sc);
	}


	//Si tiene childs los recorre y va creando sus gameobjects y componentes
	for (json::iterator iter = gameObject_json["Childs"].begin(); iter != gameObject_json["Childs"].end(); ++iter) {
		GameObject* child = createGameObject((*iter), scene, ob->getName());
		child->asingFather(ob);
		scene->addGameObject(child);
	}

	for (json::iterator itComponent = gameObject_json["Components"].begin(); itComponent != gameObject_json["Components"].end(); ++itComponent) {
		addComponent(*itComponent, ob, scene);
	}
	
	return ob;
}

GameObject * SceneLoader::createGameObjectFromPrefab(json prefabs, json gameObject_json, Scene * scene)
{
	GameObject* ob = new GameObject();
	std::string name = gameObject_json["Name"].get<std::string>();

	//Si el objeto de escena tiene parámetro mesh ignora el del prefab
	std::string meshName;
	if (gameObject_json.find("Mesh") != gameObject_json.end()) {
		meshName = gameObject_json["Mesh"].get<std::string>();
	}
	else {
		meshName = prefabs["Mesh"].get<std::string>();
	}
	std::vector<float> position;
	//Ignora la posicion del prefab si existe
	if (gameObject_json.find("Position") != gameObject_json.end()) {
		position = gameObject_json["Position"].get<std::vector<float>>();
	}
	else {
		position = prefabs["Position"].get<std::vector<float>>();
	}


	if (meshName == "SelectedShip") {
		meshName = playerShip + ".mesh";
	}
	//Si no tiene mesh crea una entidad vacía
	if (meshName == "None") {
		ob->createEmptyEntity(name, scene);
	}
	else ob->createEntity(meshName, name, scene);

	Vec3 pos(position[0], position[1], position[2]);
	ob->setPosition(pos);
	//Si tiene el parámetro escala lo asigna
	if (prefabs.find("Scale") != prefabs.end()) {
		std::vector<float> scale = prefabs["Scale"].get<std::vector<float>>();
		Vec3 sc(scale[0], scale[1], scale[2]);
		ob->setScale(sc);
	}


	//Si tiene childs los recorre y va creando sus gameobjects y componentes
	for (json::iterator iter = prefabs["Childs"].begin(); iter != prefabs["Childs"].end(); ++iter) {
		GameObject* child = createGameObject((*iter), scene, ob->getName());
		child->asingFather(ob);
		scene->addGameObject(child);
	}
	   	
	//Recorre la lista de componentes del prefab
	for (json::iterator pitComponent = prefabs["Components"].begin(); pitComponent != prefabs["Components"].end(); ++pitComponent) {
		std::string prefabComponentName = (*pitComponent)["Name"];
		bool overwrite = false;
		//Si el nombre del component del prefab coincide con el de la escena entonces se creará el componente de la escena
		json::iterator itComponent = gameObject_json["Components"].begin();
		while (itComponent != gameObject_json["Components"].end() && !overwrite) {
			std::string componentName = (*itComponent)["Name"];
			if (prefabComponentName == componentName) {
				overwrite = true;
			}
			else ++itComponent;
		}
		if (overwrite) {
			addComponent(*itComponent, ob, scene);
		}
		else {
			addComponent(*pitComponent, ob, scene);
		}
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
	bool visible = true;
	if (gui_json.find("Visible") != gui_json.end()) {
		visible = gui_json["Visible"];
	}
	if (guiType == "ImageBox") {
		std::string source = gui_json["Src"].get<std::string>();
		MyGUI::ImageBox* b = GUIManager::instance()->createImage(source, position[0], position[1], size[0], size[1], guiType, guiName);
		b->setVisible(visible);
		scene->addGUIObject(b);
	}
	else if (guiType == "TextBox") {
		std::string caption = gui_json["Caption"].get<std::string>();
		int fontHeight = gui_json["FontHeight"].get<int>();
		GUIManager::instance()->createTextbox(caption, position[0], position[1], size[0], size[1], guiType, guiName, fontHeight);
	}
}

void SceneLoader::addComponent(json object_json, GameObject * go, Scene* scene)
{	
	std::string componentName = object_json["Name"];
	if (componentName == "Camera") {
		//Adjuntamos un gameobject a la cámara creada
		std::string cameraName = object_json["CameraName"];
		Ogre::Camera* cam = scene->getSceneManager()->getCamera(cameraName);
		int dist = object_json["NearClipDistance"].get<int>();
		std::vector<int> look = object_json["LookAt"].get<std::vector<int>>();
		go->lookAt(Vec3(look[0], look[1], look[2]));
		cam->setNearClipDistance(dist);			
		go->attachCamera(cam);		
	}
	else if (componentName == "CameraMovement") {
		std::string pName = object_json["Pivot"];
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		GameObject* pivot = scene->getGameObject(pName);
		CameraMovement* cM = new CameraMovement(go, target, pivot);

		float horizontalLimit = object_json["horizontalLimit"];
		float horizontaVel = object_json["horizontaVel"];
		float horizontalBackVel = object_json["horizontalBackVel"];
		float verticalLimit = object_json["verticalLimit"];
		float verticalVel = object_json["verticalVel"];
		float aceleratedCameraOffset = object_json["aceleratedCameraOffset"];
		float deceleratedCameraOffset = object_json["deceleratedCameraOffset"];
		float cameraDefaulOffset = object_json["cameraDefaulOffset"];
		float cameraAceleratedVel = object_json["cameraAceleratedVel"];
		float cameraDeceletatedVel = object_json["cameraDeceletatedVel"];
		float maxShakeDuration = object_json["maxShakeDuration"];
		float shakeAmount = object_json["shakeAmount"];

		cM->setInitialValues(horizontalLimit, horizontaVel, horizontalBackVel, verticalLimit, verticalVel, aceleratedCameraOffset, deceleratedCameraOffset,
			cameraDefaulOffset, cameraAceleratedVel, cameraDeceletatedVel, maxShakeDuration, shakeAmount);

		scene->addComponent(cM);
	}
	else if (componentName == "ShipController") {
		int health = object_json["Health"];
		float rollingC = object_json["RollingCooldown"];
		int WZL = object_json["WarningZoneLength"];
		int DZL = object_json["DeadZoneLength"];
		ShipController* sc = new ShipController(go, shipStats[0]*health, playerShip, rollingC, WZL, DZL);

		scene->addComponent(sc);
	}
	else if (componentName == "ShotBehaviour") {
		//Le pasamos el nombre de la nave actual y los pivotes de donde saldran las balas
		std::string lName = object_json["LeftPivot"];
		GameObject* lPivot = scene->getGameObject(lName);

		std::string rName = object_json["RightPivot"];
		GameObject* rPivot = scene->getGameObject(rName);

		int damage = object_json["Damage"],
			maxOv = object_json["MaxOverload"];
		float shotCD = object_json["ShootCooldown"],
			ovRecharge = object_json["OverloadRechargeMultiplier"];
		ShotBehaviour* sb = new ShotBehaviour(go, playerShip, shipStats[1]*damage, maxOv, shotCD, ovRecharge);
		scene->addComponent(sb);
	}
	else if (componentName == "TargetController") {
		std::string cameraName = object_json["CameraName"];
		Ogre::Camera* mCamera = scene->getSceneManager()->getCamera(cameraName);
		std::string blueImg = object_json["BlueImage"];
		std::string redImg = object_json["RedImage"];
		std::string guiN = object_json["GUIName"];
		int w = object_json["Width"];
		int h = object_json["Height"];
		TargetController* tc = new TargetController(go, mCamera, blueImg,redImg,guiN,w, h);
		scene->addComponent(tc);
	}
	else if (componentName == "EnemyShoot") {
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		std::string bulletMesh = object_json["BulletMesh"];
		std::string eType = object_json["EnemyType"];
		int damage = object_json["Damage"];
		int range = object_json["Range"];
		float cd = object_json["ShootCd"];
		enemyType type;
		if (eType == "GroundTurret") {
			type = enemyType::groundTurret;
		}
		else type = enemyType::Flyer;
		EnemyShoot* FES = new EnemyShoot(go, type, target,damage, range, bulletMesh,cd);
		scene->addComponent(FES);
	}
	else if (componentName == "TurretBehaviour") {
		//Le pasamos el nombre de la nave actual
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		TurretBehaviour* tB = new TurretBehaviour(go, target);
		scene->addComponent(tB);
	}
	else if (componentName == "FlyerBehaviour") {
		//Le pasamos el nombre de la nave actual
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		int r = object_json["Radius"];
		int h = object_json["Height"];

		std::string route = object_json["Route"];

		FlyerRoute fRoute = Sinusoidal;
		if (route == "XCircular") {
			fRoute = XCircular;
		}
		else if (route == "YCircular") {
			fRoute = YCircular;
		}
		else if (route == "ZCircular") {
			fRoute = ZCircular;
		}
		else if (route == "Vertical") {
			fRoute = Vertical;
		}
		else if (route == "Horizontal") {
			fRoute = Horizontal;
		}
		else if (route == "Transversal") {
			fRoute = Transversal;
		}
		else if (route == "Sinusoidal") {
			fRoute = Sinusoidal;
		}
		FlyerBehaviour* tB = new FlyerBehaviour(go, target, fRoute, r, h);
		scene->addComponent(tB);
	}
	else if (componentName == "Rigidbody") {
		bool kinematic = false;
		RigidBody* rb ;
		int y = 0;
		int x = 0;
		if (object_json.find("Kinematic") != object_json.end()) {
			kinematic = object_json["Kinematic"];
		}
		if (object_json.find("Density") != object_json.end()) {
			int density = object_json["Density"];
			rb = new RigidBody(go, go->getName(), density, kinematic);
		}
		else {
			rb = new RigidBody(go, go->getName());
		}
		if (object_json.find("YPivot") != object_json.end()) {
			std::string yPiv = object_json["YPivot"];
			if (yPiv == "Down") {
				y = go->getBoundingBox().y / 2;
			}
			else if(yPiv=="Up"){
				y = -go->getBoundingBox().y / 2;
			}
		}
		if (object_json.find("XPivot") != object_json.end()) {
			std::string yPiv = object_json["XPivot"];
			if (yPiv == "Right") {
				x = -go->getBoundingBox().x / 2;
			}
			else if (yPiv == "Left") {
				x = go->getBoundingBox().x / 2;
			}
		}
		rb->setOffset(x, y);
		scene->addComponent(rb);
	}
	else if (componentName == "EnemyBehaviour") {
		int health = object_json["Health"];
		EnemyBehaviour* enem = new EnemyBehaviour(go, health);
		enemies.push_back(enem);
		scene->addComponent(enem);
	}
	else if (componentName == "PauseMenuManager") {
		std::string cameraName = object_json["CameraObject"];
		float cameraVel = object_json["CameraVel"];
		int buttonAmp = object_json["ButtonAmplitude"];
		float buttonSinPeriod = object_json["ButtonSinPeriod"];
		GameObject* cam = scene->getGameObject(cameraName);
		PauseMenuManager* enem = new PauseMenuManager(go, cam, cameraVel);
		enem->setButtonAmplitude(buttonAmp);
		enem->setButtonSinPeriod(buttonSinPeriod);
		scene->addComponent(enem);
	}
	else if (componentName == "EnemyManager") {
		EnemyManager* em = new EnemyManager(go);
		for (int i = 0; i < enemies.size(); i++) {
			em->addEnemy(enemies[i]);
		}
		scene->addComponent(em);
	}
	else if (componentName == "GameGUI") {
		int fH = object_json["Health"];
		GameGUI* GG = new GameGUI(go, shipStats[0] * fH);
		scene->addComponent(GG);
	}
	else if (componentName == "GameManager") {
		int levels = object_json["MaxLevels"];
		int time = object_json["WaitTime"];
		GameManager* gm = new GameManager(go,levels, time);
		scene->addComponent(gm);
	}
	else if (componentName == "Light") {
		Ogre::Light* luz = scene->getSceneManager()->createLight("Luz");
		if (object_json["Type"]=="Directional") {
			luz->setType(Ogre::Light::LT_DIRECTIONAL);
		}
		luz->setDiffuseColour(0.75, 0.75, 0.75);
		go->attachLight(luz);
	}
	else if (componentName == "PlayerController") {
		int velocity = object_json["Velocity"];
		PlayerController* pc = new PlayerController(go, shipStats[2]*velocity);
		scene->addComponent(pc);
	}
	else if (componentName == "MainMenuManager") {
		int titleAmp = object_json["TitleAmplitude"];
		float titleSinPeriod = object_json["TitleSinPeriod"];
		int buttonAmp = object_json["ButtonAmplitude"];
		float buttonSinPeriod = object_json["ButtonSinPeriod"];
		std::string cameraName = object_json["CameraObject"];
		float cameraVel = object_json["CameraVel"];
		GameObject* cam = scene->getGameObject(cameraName);
		MainMenuManager* MMM = new MainMenuManager(go, cam);
		MMM->setTitleAmplitude(titleAmp);
		MMM->setTitleSinPeriod(titleSinPeriod);
		MMM->setButtonAmplitude(buttonAmp);
		MMM->setButtonSinPeriod(buttonSinPeriod);
		MMM->setCameraVelocity(cameraVel);
		scene->addComponent(MMM);
	}
	else if (componentName == "CreditsManager") {
		int vel = object_json["Velocity"];
		int YLimit = object_json["YLimit"];
		CreditsManager* CM = new CreditsManager(go, vel, YLimit);
		scene->addComponent(CM);
	}
	else if (componentName == "ShipSelection") {
		int shipDist = object_json["ShipDistance"];
		std::string circlePivot = object_json["Pivot"];
		GameObject* ob = scene->getGameObject(circlePivot);
		ShipSelection* SS = new ShipSelection(go, shipDist, ob);
		std::vector<std::string> models = object_json["Models"];
		for (auto modelName:models) {
			GameObject* ob = scene->getGameObject(modelName);
			SS->addShipModel(ob);
		}
		std::vector<std::string> names = object_json["Names"];
		for (auto name : names) {
			SS->addShipName(name);
		}
		std::vector<std::vector<int>> stats = object_json["Stats"];
		SS->addShipStats(stats);
		SS->setInitialShipsPosition();
		scene->addComponent(SS);
	}
	
}

Scene* SceneLoader::getScene(std::string sceneName)
{
	Scene* escena = scenesMap[sceneName];
	return escena;
}


SceneLoader::~SceneLoader()
{
	scenesMap.clear();
}
