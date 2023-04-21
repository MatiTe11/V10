#pragma once
#include "DescriptorHeap.h"

namespace V10
{
	class ISimpleShadingObject
	{
	public:
		virtual DirectX::XMMATRIX GetModelMatrix() = 0;
		virtual DirectX::XMFLOAT3X3 GetNormalMatrix() = 0;
		virtual DescLocation GetTextureDescriptor() = 0;
		virtual void Draw(ID3D12GraphicsCommandList* cl) = 0;
	};
}