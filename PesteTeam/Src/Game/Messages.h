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
	int Life;

	DownLifeMsg(int l, string id = "QUITA_VIDA") : Message(id), Life(l) {}
};