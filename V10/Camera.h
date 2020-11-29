#pragma once
#include "stdafx.h"
#include "InputManager.h"

namespace V10
{
	class Camera
	{
	private:
		DirectX::XMVECTOR m_eyePosition;
		DirectX::XMVECTOR m_focusPoint;
		DirectX::XMVECTOR m_upDirection;
		DirectX::XMMATRIX m_viewMat;
		DirectX::XMMATRIX m_projectionMat;
		//InputManager* m_input;
		//Graphics& m_graphics;

	public:
		Camera();
		void Update(InputManager* inputs);
		DirectX::XMMATRIX GetVPmatrix() const;
	};
}
