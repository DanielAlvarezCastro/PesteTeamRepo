#include "ParticleManager.h"
ParticleManager::ParticleManager()
{
	particleNum = 0;
}


ParticleManager::~ParticleManager()
{
	durations.clear();
	particles.clear();
}
void ParticleManager::update(float t)
{
	for (int i = 0; i < particles.size(); i++) {
		//Si la partícula está activa hay que actualizar el tiempo que lleva viva
		if (particles[i]->isActive()) {
			currentDurations[i] += t;

			//Si el tiempo que lleva viva es mayor a su duración entonces se desactiva la partícula
			if (currentDurations[i] > durations[i]) {
				particles[i]->setActive(false);
				particles[i]->detachAllObjects();
				MainApp::instance()->getCurrentSceneMgr()->destroyParticleSystem(particles[i]->getName());
			}
		}
	}
}

void ParticleManager::createParticle(Vec3 position, Scene* scene, string particleName, float duration)
{
	particleNum++;
	GameObject* particleOb = new GameObject();
	string realName = particleName + to_string(particleNum);

	ParticleSystem* parSys = scene->getSceneManager()->createParticleSystem(realName, particleName);
	particleOb->createEmptyEntity(realName, scene);
	particleOb->setPosition(position);
	particleOb->attachMovableObject(parSys);

	particles.push_back(particleOb);
	durations.push_back(duration);
	currentDurations.push_back(0);
}
