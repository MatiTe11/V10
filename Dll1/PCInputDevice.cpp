#include "stdafx.h"
#include "PCInputDevice.h"
#include <WinUser.h>

V10::PCInputDevice::PCInputDevice()
{
	SetCursorPos(200, 200);
}


V10::PCInputDevice::~PCInputDevice()
{
}

double V10::PCInputDevice::XAxisMovement()
{
	return 0.01;
}

double V10::PCInputDevice::ZAxisMovement()
{
	return 0.01;
}

double V10::PCInputDevice::YawOffset()
{
	return 0.01;
}

double V10::PCInputDevice::PitchOffset()
{
	return 0.01;
}
