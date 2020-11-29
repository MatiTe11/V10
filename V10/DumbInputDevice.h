#pragma once
#include "IInputDevice.h"

class DumbInputDevice : public IInputDevice
{
	// Inherited via IInputDevice
	virtual double XAxisMovement() override;
	virtual double ZAxisMovement() override;
	virtual double YawOffset() override;
	virtual double PitchOffset() override;
};

