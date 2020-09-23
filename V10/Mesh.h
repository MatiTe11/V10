#pragma once
#include"Graphics.h"

class Mesh
{
private:
	ID3D12Resource * m_vertexBuffer;
	ID3D12Resource * m_indexBuffer;
	SimpleVertex * m_vertexData;
	int * m_indexData;
	int m_vertexCount;
	int m_indexCount;
	Graphics * m_graphics;

public:
	Mesh(Graphics * graphics);
	~Mesh();

	void Draw(ID3D12GraphicsCommandList * cl);

private:
	void SetBuffer(void * data, int dataSize, ID3D12Resource ** buffer);
};

