#include "stdafx.h"
#include "Mesh.h"

namespace V10
{
	Mesh::Mesh(Graphics& graphics) :
		m_graphics(graphics)
	{
		m_vertexData = {
		//back
		{ DirectX::XMFLOAT3(-0.5f, 1.0f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0, 0, 1) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0, 0, 1) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0, 0, 1) },
		{ DirectX::XMFLOAT3(0.5f, 1.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0, 0, 1) },
		//left
		{ DirectX::XMFLOAT3(-0.5f, 1.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1, 0, 0) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1, 0, 0) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1, 0, 0) },
		{ DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1, 0, 0) },
		//right
		{ DirectX::XMFLOAT3(0.5f, 1.0f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(0.5f, 1.0f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1, 0, 0) },
		//front
		{ DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0, 0, -1) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0, 0, -1) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0, 0, -1) },
		{ DirectX::XMFLOAT3(0.5f, 1.0f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0, 0, -1) } };

		m_indexData = { 0,2,1,0,1,3,   4,6,5,7,6,4,  8,9,10,11,8,10,  12,13,14,12,15,13 };

		m_vertexCount = m_vertexData.size();
		m_indexCount = m_indexData.size();

		SetBuffer(m_vertexData.data(), m_vertexCount * sizeof(SimpleVertex), &m_vertexBuffer, false);
		SetBuffer(m_indexData.data(), m_indexCount * sizeof(int), &m_indexBuffer, true);

		m_vertexView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexView.SizeInBytes = sizeof(SimpleVertex) * m_vertexCount;
		m_vertexView.StrideInBytes = sizeof(SimpleVertex);

		m_indexView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexView.SizeInBytes = sizeof(int) * m_indexCount;
		m_indexView.Format = DXGI_FORMAT_R32_UINT;
	}

	Mesh::Mesh(Graphics& graphics, aiMesh* mesh)
		:m_graphics(graphics)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			SimpleVertex vertex;
			vertex.position = DirectX::XMFLOAT3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			vertex.tangent = DirectX::XMFLOAT3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			if (mesh->mTextureCoords[0])
				vertex.texCoord = DirectX::XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			else
				vertex.texCoord = DirectX::XMFLOAT2(0, 0);

			m_vertexData.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				m_indexData.push_back(face.mIndices[j]);
		}


		m_vertexCount = m_vertexData.size();
		m_indexCount = m_indexData.size();

		SetBuffer(m_vertexData.data(), m_vertexCount * sizeof(SimpleVertex), &m_vertexBuffer, false);
		SetBuffer(m_indexData.data(), m_indexCount * sizeof(int), &m_indexBuffer, true);

		m_vertexView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexView.SizeInBytes = sizeof(SimpleVertex) * m_vertexCount;
		m_vertexView.StrideInBytes = sizeof(SimpleVertex);

		m_indexView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexView.SizeInBytes = sizeof(int) * m_indexCount;
		m_indexView.Format = DXGI_FORMAT_R32_UINT;
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Draw(ID3D12GraphicsCommandList* cl)
	{
		cl->IASetVertexBuffers(0, 1, &m_vertexView);
		cl->IASetIndexBuffer(&m_indexView);
		cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cl->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
	}

	void Mesh::SetBuffer(void* data, int dataSize, ID3D12Resource** buffer, bool index)
	{
		ID3D12Resource* bufferUploadHeap;
		auto desc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);
		*buffer = m_graphics.CreateResource(desc, D3D12_RESOURCE_STATE_COPY_DEST);
		bufferUploadHeap = m_graphics.CreateUploadBuffer(dataSize);

		void* cpuBuffer;
		bufferUploadHeap->Map(0, NULL, &cpuBuffer);
		memcpy(cpuBuffer, data, dataSize);
		bufferUploadHeap->Unmap(0, NULL);

		CommandList* commandlist = m_graphics.GetCommandList();
		auto cl = commandlist->GetCommandList();

		cl->CopyBufferRegion(*buffer, 0, bufferUploadHeap, 0, dataSize);

		if (index)
		{
			auto barrier = Graphics::GetTransition(*buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			cl->ResourceBarrier(1, &barrier);
		}
		else
		{
			auto barrier = Graphics::GetTransition(*buffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			cl->ResourceBarrier(1, &barrier);
		}

		cl->Close();
		m_graphics.Execute(commandlist);//TODO: Ÿle zrobione

		//bufferUploadHeap->Release(); //nie ruszaæ bo trójk¹t robi siê czarny (nwm czemu)
	}
}