#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "Mesh.h"


class Drawable
{
private:
	Graphics * m_graphics;
	ID3D12RootSignature * m_rootSignature;
	ID3D12PipelineState * m_pso;
	Mesh * m_mesh;
	DirectX::XMMATRIX m_modelMat;
	DirectX::XMMATRIX m_viewMat;
	DirectX::XMMATRIX m_projectionMat;
	ID3D12Resource * m_vertexBuffer;

public:
	Drawable(Graphics * graphics);
	~Drawable();

	void Draw(ID3D12GraphicsCommandList* commandlist);
	void Update(float elapsedSeconds);

private:
	void CreateRootSig();
	void CreatePSO();
};

