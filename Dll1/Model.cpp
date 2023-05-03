#include "stdafx.h"
#include "Model.h"
#include "Texture2D.h"
#include "DescriptorHeap.h"
#include "Mesh.h"

namespace V10
{

	Model::Model(Graphics& graphics, std::string name)
		:m_graphics(graphics)
	{
		Assimp::Importer import;
		std::string modelPath = name + ".obj";
		std::string diffusePath = name + "Diff.png";
		std::string normalPath = name + "Normal.png";
		const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw std::string("error");
		}

		ProcessNode(scene->mRootNode, scene);

		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 2);
		m_Textures = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), diffusePath);
		auto path = std::filesystem::current_path();
		path /= normalPath;
		if(std::filesystem::exists(path))
			m_NormalTextures = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), normalPath);

	}

	Model::~Model()
	{
	}

	void Model::Update(double elapsedSeconds)
	{
		//const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 1);
		//m_modelMat = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(0));
	}
	void Model::ResetTransform()
	{
		auto vec = DirectX::XMVectorSet(0, 0, 0, 1);
		m_modelMat = DirectX::XMMatrixTranslationFromVector(vec);
	}
	void Model::Move(DirectX::XMVECTOR translation)
	{
		//m_modelMat = DirectX::XMMatrixTranslationFromVector(translation);

		m_modelMat = DirectX::XMMatrixMultiply(m_modelMat, DirectX::XMMatrixTranslationFromVector(translation));
	}
	void Model::Rotate(DirectX::FXMVECTOR axis, float angle)
	{
		m_modelMat = DirectX::XMMatrixMultiply(m_modelMat, DirectX::XMMatrixRotationNormal(axis, angle));
	}
	void Model::Draw(ID3D12GraphicsCommandList* cl)
	{
		for(auto& mesh : m_Meshes)
			mesh.Draw(cl);
	}
	DirectX::XMMATRIX Model::GetModelMatrix()
	{
		return m_modelMat;
	}
	DirectX::XMFLOAT3X3 Model::GetNormalMatrix()
	{
		using namespace DirectX;
		auto mat4 = XMMatrixTranspose(XMMatrixInverse(nullptr, m_modelMat));
		return XMFLOAT3X3(XMVectorGetX(mat4.r[0]), XMVectorGetY(mat4.r[0]), XMVectorGetZ(mat4.r[0]),
							XMVectorGetX(mat4.r[1]), XMVectorGetY(mat4.r[1]), XMVectorGetZ(mat4.r[1]),
							XMVectorGetX(mat4.r[2]), XMVectorGetY(mat4.r[2]), XMVectorGetZ(mat4.r[2]));
	}
	DescLocation Model::GetTextureDescriptor()
	{
		return m_Textures->GetDescHandle();
	}
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.emplace_back(m_graphics, mesh);
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
}