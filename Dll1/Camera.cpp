#include "stdafx.h"
#include "Camera.h"
#include "InputManager.h"

namespace V10
{
	Camera::Camera()
	{
		m_eyePosition = DirectX::XMVectorSet(-10, 0, -10, 1);
		m_upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
		constexpr auto angle = DirectX::XMConvertToRadians(60.0f);
		m_projectionMat = DirectX::XMMatrixPerspectiveFovLH(angle, (16.0f / 9.0f), 0.1f, 100.0f);
		m_lookAt = DirectX::XMVectorSet(1, 0, 1, 1);

		Update();
	}

	void Camera::Update()
	{
		m_viewMat = DirectX::XMMatrixLookAtLH(m_eyePosition, DirectX::XMVectorAdd(m_eyePosition, DirectX::XMVector3Normalize(m_lookAt)), m_upDirection);
		m_VPMat = DirectX::XMMatrixMultiply(m_viewMat, m_projectionMat);
	}

	const DirectX::XMMATRIX& Camera::GetVPmatrix() const
	{
		return m_VPMat;
	}

	const DirectX::XMVECTOR& Camera::GetPosition() const
	{
		return m_eyePosition;
	}
	void Camera::SetEyePosition(const DirectX::XMVECTOR& eyePos)
	{
		m_eyePosition = eyePos;
	}
	void Camera::SetLookAt(const DirectX::XMVECTOR& lookAt)
	{
		m_lookAt = lookAt;
	}
}