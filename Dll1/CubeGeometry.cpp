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
		m_Material = Material{ 1,1,1 };
	}

	CubeGeometry::CubeGeometry(Graphics& graphics, std::string tex)
		:m_graphics(graphics),m_NormalTexture(nullptr)
	{
		std::string diffusePath = tex + ".png";

		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 1);
		m_Texture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), diffusePath);
		m_Mesh = std::make_unique<Mesh>(graphics);
		m_Material = Material{ 1,1,1 };

	}

	CubeGeometry::CubeGeometry(Graphics& graphics, std::string tex, std::string normalTex)
		:m_graphics(graphics)
	{
		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 2);
		m_Texture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), tex);
		m_NormalTexture = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), normalTex);
		m_Mesh = std::make_unique<Mesh>(graphics);
		m_Material = Material{ 0.1, 1, 1 };

	}

	CubeGeometry::~CubeGeometry()
	{
	}
	void CubeGeometry::Update(double elapsedSeconds)
	{
		const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 1);
		//m_modelMat = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(0));
	}
	void CubeGeometry::ResetTransform()
	{
		auto vec = DirectX::XMVectorSet(0, 0, 0, 1);
		m_modelMat = DirectX::XMMatrixTranslationFromVector(vec);
	}
	void CubeGeometry::Move(DirectX::XMVECTOR translation)
	{
		//m_modelMat = DirectX::XMMatrixTranslationFromVector(translation);

		m_modelMat = DirectX::XMMatrixMultiply(m_modelMat, DirectX::XMMatrixTranslationFromVector(translation));
	}
	void CubeGeometry::Rotate(DirectX::FXMVECTOR axis, float angle)
	{
		m_modelMat = DirectX::XMMatrixMultiply(m_modelMat, DirectX::XMMatrixRotationNormal(axis, angle));
	}
	void CubeGeometry::Scale(DirectX::FXMVECTOR axis)
	{
	}
	void CubeGeometry::Draw(ID3D12GraphicsCommandList* cl)
	{
		m_Mesh->Draw(cl);
	}
	DirectX::XMMATRIX CubeGeometry::GetModelMatrix()
	{
		return m_modelMat;
	}
	DirectX::XMMATRIX CubeGeometry::GetNormalMatrix()
	{
		using namespace DirectX;
		auto mat4 = XMMatrixTranspose(XMMatrixInverse(nullptr, m_modelMat));
		return mat4;
		/*return XMFLOAT3X3(XMVectorGetX(mat4.r[0]), XMVectorGetY(mat4.r[0]), XMVectorGetZ(mat4.r[0]),
			XMVectorGetX(mat4.r[1]), XMVectorGetY(mat4.r[1]), XMVectorGetZ(mat4.r[1]),
			XMVectorGetX(mat4.r[2]), XMVectorGetY(mat4.r[2]), XMVectorGetZ(mat4.r[2]));*/
	}
	DescLocation CubeGeometry::GetTextureDescriptor()
	{
		return m_Texture->GetDescHandle();
	}
	const Material& CubeGeometry::GetMaterial()
	{
		return m_Material;
	}
}