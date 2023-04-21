#pragma once
#include "stdafx.h"
#include "Graphics.h"

namespace V10
{
	class Mesh;
	class DescriptorHeap;
	class ISimpleShadingObject;

	class DrawExecutor
	{
	public:
		struct Shaders {
			std::string vertexShader;
			std::string pixelShader;
		};
	private:
		Graphics& m_graphics;
		ID3D12RootSignature* m_rootSignature;
		ID3D12PipelineState* m_pso;
		std::vector<std::shared_ptr<ISimpleShadingObject>> m_drawableObjects;

	public:
		DrawExecutor(Graphics& graphics, Shaders shaders, std::vector<D3D12_ROOT_PARAMETER> rootParams);
		~DrawExecutor();

		void PushDrawableObject(std::shared_ptr<ISimpleShadingObject> obj);
		void Draw(ID3D12GraphicsCommandList* commandlist, Camera* cam);
		void Update(float elapsedSeconds);

		static std::vector<D3D12_ROOT_PARAMETER> GetRootParamsForNormalMap();
		static std::vector<D3D12_ROOT_PARAMETER> GetRootParamsNoNormalMap();

	private:
		void CreateRootSig(std::vector<D3D12_ROOT_PARAMETER> rootParams);
		void CreatePSO(Shaders shaders);
	};

}