#include "stdafx.h"
#include "DumbInputDevice.h"

double DumbInputDevice::XAxisMovement()
{
	return 0.0001;
}

double DumbInputDevice::ZAxisMovement()
{
	return 0.001;
}

double DumbInputDevice::YawOffset()
{
	return 0.01;
}

double DumbInputDevice::PitchOffset()
{
	return 0.01;
}
