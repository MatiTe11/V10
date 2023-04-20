#pragma once
#include "IInputDevice.h"
#include "InputInterface.h"

namespace V10
{
	class XboxInputDevice :
		public InputInterface, public IInputDevice
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

		// Inherited via InputInterface
		virtual bool isKeyPressed(Button b) override;
	};

}