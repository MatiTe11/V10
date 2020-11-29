#pragma once
#include "IInputDevice.h"

namespace V10
{
	class XboxInputDevice :
		public IInputDevice
	{
	private:
		bool m_connectedControllers[XUSER_MAX_COUNT];
		XINPUT_STATE m_controllerState[XUSER_MAX_COUNT];
	public:
		XboxInputDevice();
		~XboxInputDevice();
		// Inherited via IInputDevice
		virtual double XAxisMovement() override;
		virtual double ZAxisMovement() override;
		virtual double YawOffset() override;
		virtual double PitchOffset() override;
	};

}