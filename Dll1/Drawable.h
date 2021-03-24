#pragma once
#include "stdafx.h"
#include "Graphics.h"

namespace V10
{
	class Mesh;
	class DescriptorHeap;
	class ISimpleShadingObject;

	class Drawable // TODO: Change name of this class
	{
	private:
		Graphics& m_graphics;
		ID3D12RootSignature* m_rootSignature;
		ID3D12PipelineState* m_pso;
		std::vector<ISimpleShadingObject*> m_drawableObjects;

	public:
		Drawable(Graphics& graphics);
		~Drawable();

		void PushDrawableObject(ISimpleShadingObject* obj);
		void Draw(ID3D12GraphicsCommandList* commandlist, Camera* cam);
		void Update(float elapsedSeconds);

	private:
		void CreateRootSig();
		void CreatePSO();
	};

}