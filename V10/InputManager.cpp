#include "stdafx.h"
#include "InputManager.h"


InputManager::InputManager()
{
	DWORD dwResult;

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_connectedControllers[i] = false;
	}

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&m_controllerState[i], sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &m_controllerState[i]);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			m_connectedControllers[i] = true;
		}
		else
		{
			// Controller is not connected
			m_connectedControllers[i] = false;
		}
	}
}


InputManager::~InputManager()
{
}

void InputManager::Update()
{
}
