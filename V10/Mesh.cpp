#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(Graphics * graphics):
	m_graphics(graphics)
{
	SimpleVertex vertexData[] = {
	{ DirectX::XMFLOAT3(-0.5f, 1.0f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
	{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
	{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
	{ DirectX::XMFLOAT3(0.5f, 1.0f, 0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) } };

	unsigned int indexData[] = { 0,1,2,0,3,1 };

	m_vertexCount = _countof(vertexData);
	m_indexCount = _countof(indexData);

	SetBuffer(vertexData, m_vertexCount * sizeof(SimpleVertex), &m_vertexBuffer, false);
	SetBuffer(indexData, m_indexCount * sizeof(int), &m_indexBuffer, true);

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

void Mesh::Draw(ID3D12GraphicsCommandList * cl)
{
	cl->IASetVertexBuffers(0, 1, &m_vertexView);
	cl->IASetIndexBuffer(&m_indexView);
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cl->DrawIndexedInstanced(m_indexCount, 1, 0, 0, 0);
}

void Mesh::SetBuffer(void * data, int dataSize, ID3D12Resource ** buffer, bool index)
{
	ID3D12Resource* bufferUploadHeap;
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);
	*buffer = m_graphics->CreateResource(desc, D3D12_RESOURCE_STATE_COPY_DEST);
	bufferUploadHeap = m_graphics->CreateUploadBuffer(dataSize);

	void * cpuBuffer;
	bufferUploadHeap->Map(0, NULL, &cpuBuffer);
	memcpy(cpuBuffer, data, dataSize);
	bufferUploadHeap->Unmap(0, NULL);

	CommandList* commandlist = m_graphics->GetCommandList();
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
	m_graphics->Execute(commandlist);//TODO: Ÿle zrobione

	//bufferUploadHeap->Release(); //nie ruszaæ bo trójk¹t robi siê czarny (nwm czemu)
}
