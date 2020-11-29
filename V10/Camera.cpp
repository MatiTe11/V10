#include "stdafx.h"
#include "Camera.h"

namespace V10
{
	Camera::Camera()
	{

		m_eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
		m_focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
		m_upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
	}

	void Camera::Update(InputManager* input)
	{
		auto cameraFront = input->GetPosition();
		auto offset = DirectX::XMVectorSet(0, 0, 1, 1);
		cameraFront = DirectX::XMVectorAdd(cameraFront, offset);

		m_eyePosition = input->GetPosition();
		m_viewMat = DirectX::XMMatrixLookAtLH(m_eyePosition, DirectX::XMVectorAdd(m_eyePosition, input->GetCameraFront()), m_upDirection);
		m_projectionMat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (16.0f / 9.0f), 0.1f, 100.0f);
	}

	DirectX::XMMATRIX Camera::GetVPmatrix() const
	{
		return DirectX::XMMatrixMultiply(m_viewMat, m_projectionMat);
	}
}