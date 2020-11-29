#pragma once
#include "stdafx.h"
#include "Graphics.h"
#include "Mesh.h"


class Drawable
{
private:
	Graphics& m_graphics;
	ID3D12RootSignature * m_rootSignature;
	ID3D12PipelineState * m_pso;
	ID3D12DescriptorHeap* m_descHeap;
	std::unique_ptr<Mesh> m_mesh;
	DirectX::XMMATRIX m_modelMat;

public:
	Drawable(Graphics& graphics);
	~Drawable();

	void Draw(ID3D12GraphicsCommandList* commandlist, Camera * cam);
	void Update(float elapsedSeconds);

private:
	void CreateRootSig();
	void CreatePSO();
	void CreateDescHeap();
};

