#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(Graphics * graphics):
	m_graphics(graphics)
{
	SimpleVertex vertexData[] = {
	{ DirectX::XMFLOAT3(-0.5f, 1.0f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
	{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
	{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) } };

	m_vertexCount = _countof(vertexData);
	SetBuffer(vertexData, m_vertexCount * sizeof(SimpleVertex), &m_vertexBuffer);
}


Mesh::~Mesh()
{
}

void Mesh::Draw(ID3D12GraphicsCommandList * cl)
{
	D3D12_VERTEX_BUFFER_VIEW vertView;
	vertView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
	vertView.SizeInBytes = sizeof(SimpleVertex) * m_vertexCount;
	vertView.StrideInBytes = sizeof(SimpleVertex);

	cl->IASetVertexBuffers(0, 1, &vertView);
	cl->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cl->DrawInstanced(m_vertexCount, 1, 0, 0);
}

void Mesh::SetBuffer(void * data, int dataSize, ID3D12Resource ** buffer)
{
	ID3D12Resource* bufferUploadHeap;
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(dataSize);
	*buffer = m_graphics->CreateResource(desc, D3D12_RESOURCE_STATE_COPY_DEST);
	bufferUploadHeap = m_graphics->CreateUploadBuffer(dataSize);

	void * cpuBuffer;
	bufferUploadHeap->Map(0, NULL, &cpuBuffer);
	memcpy(cpuBuffer, data, dataSize);
	bufferUploadHeap->Unmap(0, NULL);

	m_graphics->GetInitCommandList()->CopyBufferRegion(m_vertexBuffer, 0, bufferUploadHeap, 0, dataSize);
	auto barrier = Graphics::GetTransition(m_vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	m_graphics->GetInitCommandList()->ResourceBarrier(1, &barrier);

	//bufferUploadHeap->Release();
}
