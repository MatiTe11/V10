#pragma once

namespace V10
{
	class IInputDevice
	{
	public:
		virtual double XAxisMovement() = 0;
		virtual double ZAxisMovement() = 0;
		virtual double YawOffset() = 0;
		virtual double PitchOffset() = 0;
	};
}