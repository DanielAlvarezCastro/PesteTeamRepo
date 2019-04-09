#pragma once
#include <MainApp.h>
#include <BasicComponent.h>

struct TiltCameraBackMsg : public Message
{
	Radian radian;

	TiltCameraBackMsg(string id, Radian negativeRadian) :Message(id), radian(negativeRadian) {};
};