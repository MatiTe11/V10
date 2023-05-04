#pragma once

namespace V10
{
	class CameraInterface
	{
	public:
		virtual void SetEyePosition(const DirectX::XMVECTOR& eyePos) = 0;
		virtual void SetLookAt(const DirectX::XMVECTOR& lookAt) = 0;
	};
}
