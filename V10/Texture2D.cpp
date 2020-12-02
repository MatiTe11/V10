#include "stdafx.h"
#include "Texture2D.h"
#include <DirectXTex.h>

namespace V10
{
	Texture2D::Texture2D(Graphics& graphics, DescLocation descHandle, std::string texName)
		:m_graphics(graphics), m_texture(nullptr), m_descHandle(descHandle)
	{
		auto path = std::filesystem::current_path();
		//path /= "tex.png";
		path /= texName;
		auto img = LoadFromFile(path.wstring());
		m_textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		m_textureDesc.Alignment = 0;
		m_textureDesc.Width = img.width;
		m_textureDesc.Height = img.height;
		m_textureDesc.DepthOrArraySize = 1;
		m_textureDesc.MipLevels = 1;
		m_textureDesc.Format = img.format;
		m_textureDesc.SampleDesc.Count = 1;
		m_textureDesc.SampleDesc.Quality = 0;
		m_textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		m_textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		m_graphics.GetDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, &m_textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_texture));

		ID3D12Resource* uploadRes;
		UINT64 textureUploadBufferSize;
		m_graphics.GetDevice()->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

		m_graphics.GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&uploadRes));
		void* data;
		uploadRes->Map(0, NULL, &data);

		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = img.pixels;
		textureData.RowPitch = img.rowPitch;
		textureData.SlicePitch = img.slicePitch;

		auto cl = m_graphics.GetCommandList();
		UpdateSubresources(cl->GetCommandList(), m_texture, uploadRes, 0, 0, 1, &textureData);

		cl->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
		cl->GetCommandList()->Close();
		m_graphics.Execute(cl);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = GetDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		m_graphics.GetDevice()->CreateShaderResourceView(m_texture, &srvDesc, descHandle.cpuHandle);

	}


	Texture2D::~Texture2D()
	{
	}

	DirectX::Image Texture2D::LoadFromFile(std::wstring filename)
	{
		using namespace DirectX;
		ScratchImage* image = new ScratchImage();
		HRESULT hr = LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, nullptr, *image);
		DirectX::Image ret = *(image->GetImage(0, 0, 0));
		return ret;
	}
}