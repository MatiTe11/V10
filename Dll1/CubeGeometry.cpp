#include "stdafx.h"
#include "CubeGeometry.h"
#include "Texture2D.h"
#include "DescriptorHeap.h"
#include "Mesh.h"

namespace V10
{
	CubeGeometry::CubeGeometry(Graphics& graphics)
		:m_graphics(graphics), m_NormalTexture(nullptr)
	{
		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 1);
		m_Texture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor());
		m_Mesh = std::make_unique<Mesh>(graphics);
	}

	CubeGeometry::CubeGeometry(Graphics& graphics, std::string tex)
		:m_graphics(graphics),m_NormalTexture(nullptr)
	{
		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 1);
		m_Texture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), tex);
		m_Mesh = std::make_unique<Mesh>(graphics);
	}

	CubeGeometry::CubeGeometry(Graphics& graphics, std::string tex, std::string normalTex)
		:m_graphics(graphics)
	{
		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 2);
		m_Texture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), tex);
		m_NormalTexture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), normalTex);
		m_Mesh = std::make_unique<Mesh>(graphics);
	}

	CubeGeometry::~CubeGeometry()
	{
	}
	void CubeGeometry::Update(double elapsedSeconds)
	{
		const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 1);
		//m_modelMat = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(0));
	}
	void CubeGeometry::Move(DirectX::XMVECTOR translation)
	{
		m_modelMat = DirectX::XMMatrixTranslationFromVector(translation);
	}
	void CubeGeometry::Draw(ID3D12GraphicsCommandList* cl)
	{
		m_Mesh->Draw(cl);
	}
	DirectX::XMMATRIX CubeGeometry::GetModelMatrix()
	{
		return m_modelMat;
	}
	DescLocation CubeGeometry::GetTextureDescriptor()
	{
		return m_Texture->GetDescHandle();
	}
}