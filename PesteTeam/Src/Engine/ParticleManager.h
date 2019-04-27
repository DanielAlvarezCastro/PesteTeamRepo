#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include "Scene.h"
using namespace std;
class ParticleManager
{
private:
	int particleNum;
	std::vector<GameObject*> particles;
	std::vector<float> durations;
	std::vector<float> currentDurations;
public:
	void update(float t);
	ParticleManager();
	~ParticleManager();
	void createParticle(Vec3 position, Scene* scene, string particleName, float duration);
};

