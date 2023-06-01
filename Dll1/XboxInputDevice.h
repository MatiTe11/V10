#pragma once
#include "IInputDevice.h"
#include "InputInterface.h"

namespace V10
{
	class XboxInputDevice :
		public InputInterface
	{
	private:
		bool m_connectedControllers[XUSER_MAX_COUNT];
		XINPUT_STATE m_controllerState[XUSER_MAX_COUNT];
	public:
		XboxInputDevice();
		~XboxInputDevice();

		void Update();

		// Inherited via InputInterface
		virtual bool isKeyPressed(Button b) override;
	};

}