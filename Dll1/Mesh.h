#pragma once
#include"Graphics.h"

namespace V10
{
	class Mesh
	{
	private:
		ID3D12Resource* m_vertexBuffer;
		ID3D12Resource* m_indexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_vertexView;
		D3D12_INDEX_BUFFER_VIEW m_indexView;

		DirectX::XMMATRIX m_modelMat;
		std::vector<SimpleVertex> m_vertexData;
		std::vector<int> m_indexData;
		int m_vertexCount;
		unsigned int m_indexCount;
		Graphics& m_graphics;

	public:
		Mesh(Graphics& graphics);
		Mesh(Graphics& graphics, aiMesh *assMesh);
		~Mesh();

		void Draw(ID3D12GraphicsCommandList* cl);

	private:
		void SetBuffer(void* data, int dataSize, ID3D12Resource** buffer, bool index);
	};

}