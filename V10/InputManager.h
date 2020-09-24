#pragma once
class InputManager
{
	bool m_connectedControllers[XUSER_MAX_COUNT];
	XINPUT_STATE m_controllerState[XUSER_MAX_COUNT];

public:
	InputManager();
	~InputManager();
	void Update();
};

