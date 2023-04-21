#include "stdafx.h"
#include "XboxInputDevice.h"

namespace V10
{
	XboxInputDevice::XboxInputDevice()
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

	XboxInputDevice::~XboxInputDevice()
	{
	}

	double XboxInputDevice::XAxisMovement()
	{
		auto dwResult = XInputGetState(0, &m_controllerState[0]);
		if (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			return -0.01;
		else if (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			return 0.01;
		else
			return 0;
	}

	double XboxInputDevice::ZAxisMovement()
	{
		auto dwResult = XInputGetState(0, &m_controllerState[0]);
		if (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			return 0.01;
		else if (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			return -0.01;
		else
			return 0.0;
	}

	double XboxInputDevice::YawOffset()
	{
		auto dwResult = XInputGetState(0, &m_controllerState[0]);
		if (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_X)
			return 0.02;
		else if (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_B)
			return -0.02;
		else
			return 0.0;
	}

	double XboxInputDevice::PitchOffset()
	{
		return 0.0;
	}
	bool XboxInputDevice::isKeyPressed(Button b)
	{
		auto dwResult = XInputGetState(0, &m_controllerState[0]);

		switch (b)
		{
		case V10::Button::UP:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
		case V10::Button::DOWN:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		case V10::Button::LEFT:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		case V10::Button::RIGHT:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
		case V10::Button::X:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_X);
		case V10::Button::Y:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_Y);
		case V10::Button::A:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_A);
		case V10::Button::B:
			return (m_controllerState[0].Gamepad.wButtons & XINPUT_GAMEPAD_B);
		default:
			return false;
		}
		return false;
	}
}