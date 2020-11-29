#include "stdafx.h"
#include "Model.h"
#include "Texture2D.h"
#include "DescriptorHeap.h"
#include "Mesh.h"

namespace V10
{
	Model::Model(Graphics& graphics)
		:m_graphics(graphics)
	{
		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 2);
		m_Texture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor());
		m_Mesh = std::make_unique<Mesh>(graphics);
	}
	Model::~Model()
	{
	}
	void Model::Update(double elapsedSeconds)
	{
		const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 1);
		//m_modelMat = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(0));
	}
	void Model::Move(DirectX::XMVECTOR translation)
	{
		m_modelMat = DirectX::XMMatrixTranslationFromVector(translation);
	}
	void Model::Draw(ID3D12GraphicsCommandList* cl)
	{
		m_Mesh->Draw(cl);
	}
	DirectX::XMMATRIX Model::GetModelMatrix()
	{
		return m_modelMat;
	}
	DescLocation Model::GetTextureDescriptor()
	{
		return m_Texture->GetDescHandle();
	}
}