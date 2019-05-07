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