#include "stdafx.h"
#include "Texture2D.h"
#include <DirectXTex.h>

Texture2D::Texture2D(Graphics* graphics, int width, int height, DXGI_FORMAT format)
	:m_graphics(graphics), m_texture(nullptr)
{
	auto path = std::filesystem::current_path();
	path /= "tex.png";
	auto img = LoadFromFile(path.wstring());
	m_textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	m_textureDesc.Alignment = 0; // may be 0, 4KB, 64KB, or 4MB. 0 will let runtime decide between 64KB and 4MB (4MB for multi-sampled textures)
	m_textureDesc.Width = img.width; // width of the texture
	m_textureDesc.Height = img.height; // height of the texture
	m_textureDesc.DepthOrArraySize = 1; // if 3d image, depth of 3d image. Otherwise an array of 1D or 2D textures (we only have one image, so we set 1)
	m_textureDesc.MipLevels = 1; // Number of mipmaps. We are not generating mipmaps for this texture, so we have only one level
	m_textureDesc.Format = img.format; // This is the dxgi format of the image (format of the pixels)
	m_textureDesc.SampleDesc.Count = 1; // This is the number of samples per pixel, we just want 1 sample
	m_textureDesc.SampleDesc.Quality = 0; // The quality level of the samples. Higher is better quality, but worse performance
	m_textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // The arrangement of the pixels. Setting to unknown lets the driver choose the most efficient one
	m_textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE; // no flags

	m_graphics->GetDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, &m_textureDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_texture));

	ID3D12Resource* uploadRes;
	UINT64 textureUploadBufferSize;
	// this function gets the size an upload buffer needs to be to upload a texture to the gpu.
	// each row must be 256 byte aligned except for the last row, which can just be the size in bytes of the row
	// eg. textureUploadBufferSize = ((((width * numBytesPerPixel) + 255) & ~255) * (height - 1)) + (width * numBytesPerPixel);
	//textureUploadBufferSize = (((imageBytesPerRow + 255) & ~255) * (textureDesc.Height - 1)) + imageBytesPerRow;
	m_graphics->GetDevice()->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &textureUploadBufferSize);

	// now we create an upload heap to upload our texture to the GPU
	m_graphics->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // upload heap
		D3D12_HEAP_FLAG_NONE, // no flags
		&CD3DX12_RESOURCE_DESC::Buffer(textureUploadBufferSize), // resource description for a buffer (storing the image data in this heap just to copy to the default heap)
		D3D12_RESOURCE_STATE_GENERIC_READ, // We will copy the contents from this heap to the default heap above
		nullptr,
		IID_PPV_ARGS(&uploadRes));
	void* data;
	uploadRes->Map(0, NULL, &data);

	// store vertex buffer in upload heap
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = img.pixels; // pointer to our image data
	textureData.RowPitch = img.rowPitch; // size of all our triangle vertex data
	textureData.SlicePitch = img.slicePitch; // also the size of our triangle vertex data

	// Now we copy the upload buffer contents to the default heap
	auto cl = m_graphics->GetCommandList();
	UpdateSubresources(cl->GetCommandList(), m_texture, uploadRes, 0, 0, 1, &textureData);

	// transition the texture default heap to a pixel shader resource (we will be sampling from this heap in the pixel shader to get the color of pixels)
	cl->GetCommandList()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	cl->GetCommandList()->Close();
	m_graphics->Execute(cl);
}


Texture2D::~Texture2D()
{
}

DirectX::Image Texture2D::LoadFromFile(std::wstring filename)
{
	using namespace DirectX;
	ScratchImage* image = new ScratchImage();
	HRESULT hr = LoadFromWICFile(filename.c_str(), WIC_FLAGS_NONE, nullptr, *image);
	DirectX::Image ret = *(image->GetImage(0,0,0));
	return ret;
}
