#include "EnemyManager.h"
#include <GameObject.h>
#include "Messages.h"

EnemyManager::EnemyManager(GameObject* go) : BehaviourComponent(go)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update(float t)
{

}

void EnemyManager::reciveMsg(Message* msg) 
{
	if (msg->id == "QUITA_VIDA") 
	{
		DownLifeMsg* dlm = static_cast<DownLifeMsg*>(msg);
		int i = 0;
		bool found = false;
		while(i < enemies_c.size() && !found) {
			if (enemies_c[i]->getGameObject()->getName() == dlm->name)
			{
				enemies_c[i]->hit(dlm->power);
				found = true;
			}
			i++;
		}
	}
}