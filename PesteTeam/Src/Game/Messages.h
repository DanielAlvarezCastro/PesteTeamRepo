#pragma once
#include <MainApp.h>
#include <BasicComponent.h>

struct TiltCameraBackMsg : public Message
{
	Radian radian;

	TiltCameraBackMsg(string id, Radian negativeRadian) :Message(id), radian(negativeRadian) {};
};

struct DownLifeMsg : public Message
{
	int power;
	string name;

	DownLifeMsg(int p, string n, string id = "QUITA_VIDA") : Message(id), power(p), name(n) {}
};
struct EnemyDeadMsg : public Message {
	EnemyDeadMsg(string id = "ENEMY_DEAD") : Message(id) {};
};
struct UpdateHealthBarMsg : public Message {
	int currentHealth;
	UpdateHealthBarMsg(int _currentHealth, string id = "UPDATE_HEALTHBAR") : Message(id), currentHealth(_currentHealth) {};
};
struct BulletCollisionMsg : public Message {
	Ogre::Vector3 pos;
	BulletCollisionMsg(Ogre::Vector3 position,string id = "BULLET_COLLISION") : Message(id), pos(position) {}
};
struct GameOverMsg : public Message
{
	GameOverMsg(string id = "GAME_OVER") : Message(id) {}
};
struct MissionAccomplishedMsg : public Message {
	MissionAccomplishedMsg(string id = "MISSION_ACCOMPLISHED") : Message(id){}
};
struct NextLevelMsg : public Message
{
	NextLevelMsg(string id = "NEXT_LEVEL") : Message(id) {}
};