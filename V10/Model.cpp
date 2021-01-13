#include "stdafx.h"
#include "Model.h"
#include "Texture2D.h"
#include "DescriptorHeap.h"
#include "Mesh.h"

namespace V10
{

	Model::Model(Graphics& graphics, std::string path)
		:m_graphics(graphics)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			throw std::string("error");
		}

		ProcessNode(scene->mRootNode, scene);


		m_descHeap = std::make_unique<DescriptorHeap>(graphics, 1);
		m_Textures = std::make_unique<Texture2D>(graphics, m_descHeap->GetNextDescriptor(), "wood.jpg");
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
		for(auto mesh : m_Meshes)
			mesh.Draw(cl);
	}
	DirectX::XMMATRIX Model::GetModelMatrix()
	{
		return m_modelMat;
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