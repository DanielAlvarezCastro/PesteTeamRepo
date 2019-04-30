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