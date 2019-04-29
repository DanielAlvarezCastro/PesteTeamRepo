#include "ParticleManager.h"
ParticleManager::ParticleManager()
{
	particleNum = 0;
}


ParticleManager::~ParticleManager()
{
	particlesMap.clear();
}
void ParticleManager::update(float t)
{
	//Recorre el diccionario de particulas
	for (std::map<std::string, std::vector<ParticleInfo>>::iterator iter = particlesMap.begin(); iter != particlesMap.end(); ++iter)
	{
		std::string particleName = iter->first;
		std::vector<ParticleInfo> particles = iter->second;
		for (int i = 0; i < particles.size(); i++) {
			//Si la part�cula est� activa hay que actualizar el tiempo que lleva viva
			if (particles[i].parSys->isVisible()) {
				particles[i].currentDuration += t;

				//Si el tiempo que lleva viva es mayor a su duraci�n entonces se desactiva la part�cula
				if (particles[i].currentDuration > particles[i].maxDuration) {
					particles[i].parSys->setVisible(false);
					particles[i].currentDuration = 0;
				}
			}
		}
		particlesMap[particleName] = particles;
	}
}

void ParticleManager::createParticle(Vec3 position, string particleName, float duration, string materialName)
{
	Scene* scene = MainApp::instance()->getCurrentScene();
	int i = 0;
	bool found = false;
	//Busca si existe una part�cula de su tipo que est� desactivada para hacer pooling
	std::vector<ParticleInfo> particles = particlesMap[particleName];
	while (i < particles.size() && !found) {
		if (!particles[i].parSys->isVisible()) {
			found = true;
		}
		else i++;		
	}
	//Si encuentra una la hace visible, reinicia su timer y se coloca en la posici�n
	if (found) {
		particles[i].parSys->setVisible(true);
		particles[i].gameObject->setPosition(position);
		particles[i].currentDuration = 0;
	}
	else {
		//Crea una part�cula nueva y la mete en el diccionario
		particleNum++;
		GameObject* particleOb = new GameObject();
		string realName = particleName + to_string(particleNum);

		ParticleSystem* parSys = scene->getSceneManager()->createParticleSystem(realName, particleName);
		particleOb->createEmptyEntity(realName, scene);
		particleOb->setPosition(position);
		particleOb->attachMovableObject(parSys);

		if (materialName != "None") {
			parSys->setMaterialName(materialName);
		}
		
		ParticleInfo pInfo;
		pInfo.gameObject = particleOb;
		pInfo.parSys = parSys;
		pInfo.maxDuration = duration;
		pInfo.currentDuration = 0;
		
		particles.push_back(pInfo);
			
		
	}
	particlesMap[particleName] = particles;
}
