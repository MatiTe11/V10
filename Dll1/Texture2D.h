#pragma once
#include "Graphics.h"
#include "DescriptorHeap.h"
#include <DirectXTex.h>

namespace V10
{
	class Texture2D
	{
	private:
		Graphics& m_graphics;
		ID3D12Resource* m_texture;
		D3D12_RESOURCE_DESC m_textureDesc;
		DescLocation m_descHandle;

	public:
		Texture2D(Graphics& graphics, DescLocation descHandle, std::string texName = "tex.png");
		~Texture2D();

		const D3D12_RESOURCE_DESC& GetDesc() { return m_textureDesc; }
		ID3D12Resource* Get() { return m_texture; }
		DescLocation GetDescHandle() { return m_descHandle; }

	private:
		DirectX::Image LoadFromFile(std::wstring filename);
	};
}

