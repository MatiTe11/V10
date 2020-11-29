#include "stdafx.h"
#include "InputManager.h"

namespace V10
{
	InputManager::InputManager(IInputDevice* inputDev)
		:m_inputDev(inputDev)
	{
		m_Position = DirectX::XMVectorSet(0, 0, -10, 1);
		m_CameraFront = DirectX::XMVectorSet(0, 0, 1, 1);
		m_yawRadians = 3.14 / 2.0;
		m_pitchRadians = 0;
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::Update(float elapsedSeconds)
	{
		auto speed = (float)m_speed * elapsedSeconds;
		auto offset = DirectX::XMVectorSet(m_inputDev->XAxisMovement() * speed, 0, m_inputDev->ZAxisMovement() * speed, 0);
		m_Position = DirectX::XMVectorAdd(m_Position, offset);
		//yaw, pitch roll
		m_yawRadians += m_inputDev->YawOffset();
		m_pitchRadians += m_inputDev->PitchOffset();
		DirectX::XMVECTOR direction;
		direction = DirectX::XMVectorSet(cos(m_yawRadians) * cos(m_pitchRadians),
			sin(m_pitchRadians),
			sin(m_yawRadians) * cos(m_pitchRadians), 1);

		m_CameraFront = DirectX::XMVector3Normalize(direction);

	}
}