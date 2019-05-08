#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include "Scene.h"
#include <map>
using namespace std;
class ParticleManager
{
private:
	//Tiene dentro la información básica de una partícula
	struct ParticleInfo {
		GameObject* gameObject;
		ParticleSystem* parSys;
		float maxDuration;
		float currentDuration;
	};
	int particleNum;
	//Diccionario con un vector de los distintos tipos de partículas activos
	std::map<std::string, std::vector<ParticleInfo>> particlesMap;
public:
	void update(float t);
	ParticleManager();
	~ParticleManager();
	void createParticle(Ogre::Vector3  position, string particleName, float duration, string materialName = "None");
};

