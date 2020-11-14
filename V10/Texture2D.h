#pragma once
#include "Graphics.h"


class Texture2D
{
private:
	Graphics* m_graphics;
	ID3D12Resource* m_texture;
	D3D12_RESOURCE_DESC textureDesc;
public:
	Texture2D(Graphics* graphics, int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT);
	~Texture2D();

	const D3D12_RESOURCE_DESC& GetDesc() { return textureDesc; }
	ID3D12Resource* Get() { return m_texture; }
};

