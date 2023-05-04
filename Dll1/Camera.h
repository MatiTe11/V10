#pragma once
#include "stdafx.h"
#include "CameraInterface.h"


namespace V10
{

	class Camera: public CameraInterface
	{
	private:
		DirectX::XMVECTOR m_eyePosition;
		DirectX::XMVECTOR m_lookAt;
		DirectX::XMVECTOR m_upDirection;
		DirectX::XMMATRIX m_viewMat;
		DirectX::XMMATRIX m_projectionMat;
		DirectX::XMMATRIX m_VPMat;

	public:
		Camera();
		void Update();
		const DirectX::XMMATRIX& GetVPmatrix() const;
		const DirectX::XMVECTOR& GetPosition() const;

		// Inherited via CameraInterface
		virtual void SetEyePosition(const DirectX::XMVECTOR& eyePos) override;
		virtual void SetLookAt(const DirectX::XMVECTOR& lookAt) override;
	};
}
