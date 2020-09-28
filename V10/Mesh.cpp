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

	D3D12_INDEX_BUFFER_VIEW indexView;
	indexView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	indexView.SizeInBytes = sizeof(int) * m_indexCount;
	indexView.Format = DXGI_FORMAT_R32_UINT;

	cl->IASetVertexBuffers(0, 1, &vertView);
	cl->IASetIndexBuffer(&indexView);
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
