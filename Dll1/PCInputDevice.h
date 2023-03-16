#pragma once
#include "IInputDevice.h"

namespace V10
{
	class PCInputDevice :
		public IInputDevice
	{
	public:
		PCInputDevice();
		~PCInputDevice();

		// Inherited via IInputDevice
		virtual double XAxisMovement() override;
		virtual double ZAxisMovement() override;
		virtual double YawOffset() override;
		virtual double PitchOffset() override;
	};
}

