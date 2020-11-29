#pragma once
#include "IInputDevice.h"

class InputManager
{
private:
	bool m_connectedControllers[XUSER_MAX_COUNT];
	XINPUT_STATE m_controllerState[XUSER_MAX_COUNT];
	IInputDevice* m_inputDev;
	DirectX::XMVECTOR m_Position;
	double m_yawRadians;
	double m_pitchRadians;
	DirectX::XMVECTOR m_CameraFront;
	const int m_speed = 10;
	const int m_sensitivity = 0;

public:
	InputManager(IInputDevice* inputDev);
	~InputManager();
	void Update(float elapsedSeconds);
	const DirectX::XMVECTOR& GetPosition() { return m_Position; }
	const DirectX::XMVECTOR& GetCameraFront() { return m_CameraFront; }

};

