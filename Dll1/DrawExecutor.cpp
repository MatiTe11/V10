#include "stdafx.h"
#include "DrawExecutor.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "DescriptorHeap.h"
#include "ISimpleShadingObject.h"

namespace V10
{
	DrawExecutor::DrawExecutor(Graphics& graphics, Shaders shaders, std::vector<D3D12_ROOT_PARAMETER> rootParams)
		:m_graphics(graphics)
	{
		CreateRootSig(rootParams);
		CreatePSO(shaders);
	}

	DrawExecutor::~DrawExecutor()
	{
	}

	void DrawExecutor::PushDrawableObject(std::shared_ptr<ISimpleShadingObject> obj)
	{
		m_drawableObjects.push_back(obj);
	}

	void DrawExecutor::Draw(ID3D12GraphicsCommandList* cl, Camera* cam)
	{
		cl->SetPipelineState(m_pso);
		cl->SetGraphicsRootSignature(m_rootSignature);

		//for each object
		for (int i = 0; i < m_drawableObjects.size(); i++)
		{
			auto modelMat = m_drawableObjects[i]->GetModelMatrix();
			auto normalMat = m_drawableObjects[i]->GetNormalMatrix();
			auto vpMat =  cam->GetVPmatrix();
			auto camPos = cam->GetPosition();
			cl->SetGraphicsRoot32BitConstants(0, sizeof(DirectX::XMMATRIX) / 4, &modelMat, 0);
			cl->SetGraphicsRoot32BitConstants(1, sizeof(DirectX::XMMATRIX) / 4, &normalMat, 0);
			cl->SetGraphicsRoot32BitConstants(2, sizeof(DirectX::XMMATRIX) / 4, &vpMat, 0);
			cl->SetGraphicsRoot32BitConstants(3, sizeof(Material) / 4, &(m_drawableObjects[i]->GetMaterial()), 0);
			cl->SetGraphicsRoot32BitConstants(4, sizeof(DirectX::XMVECTOR) / 4, &camPos, 0);
			auto desc = m_drawableObjects[i]->GetTextureDescriptor();
			cl->SetDescriptorHeaps(1, &desc.descHeap);
			cl->SetGraphicsRootDescriptorTable(5, desc.gpuHandle);
			m_drawableObjects[i]->Draw(cl);
		}
	}

	void DrawExecutor::Update(float elapsedSeconds)
	{
		/*const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 1);
		m_modelMat = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(0));*/
	}

	std::vector<D3D12_ROOT_PARAMETER> DrawExecutor::GetRootParamsForNormalMap()
	{
		std::vector<D3D12_ROOT_PARAMETER> rootParams(6);

		D3D12_ROOT_CONSTANTS modelRC{ 0 };
		modelRC.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
		modelRC.RegisterSpace = 0;
		modelRC.ShaderRegister = 0;
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[0].Constants = modelRC;
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS normalRC{ 0 };
		normalRC.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
		normalRC.RegisterSpace = 0;
		normalRC.ShaderRegister = 1;
		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[1].Constants = normalRC;
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS viewProjRC{ 0 };
		viewProjRC.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
		viewProjRC.RegisterSpace = 0;
		viewProjRC.ShaderRegister = 2;
		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[2].Constants = viewProjRC;
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS materialRC{ 0 };
		materialRC.Num32BitValues = sizeof(Material) / 4;
		materialRC.RegisterSpace = 0;
		materialRC.ShaderRegister = 3;
		rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[3].Constants = materialRC;
		rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS camPosRC{ 0 };
		camPosRC.Num32BitValues = sizeof(DirectX::XMVECTOR) / 4;
		camPosRC.RegisterSpace = 0;
		camPosRC.ShaderRegister = 4;
		rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[4].Constants = camPosRC;
		rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_DESCRIPTOR_RANGE* descriptorTableRange = new D3D12_DESCRIPTOR_RANGE;
		descriptorTableRange->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorTableRange->NumDescriptors = 2;
		descriptorTableRange->BaseShaderRegister = 0;
		descriptorTableRange->RegisterSpace = 0;
		descriptorTableRange->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
		descriptorTable.NumDescriptorRanges = 1;
		descriptorTable.pDescriptorRanges = descriptorTableRange;
		rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[5].DescriptorTable = descriptorTable;
		rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		return rootParams;

	}

	std::vector<D3D12_ROOT_PARAMETER> DrawExecutor::GetRootParamsNoNormalMap()
	{
		std::vector<D3D12_ROOT_PARAMETER> rootParams(6);

		D3D12_ROOT_CONSTANTS modelRC{ 0 };
		modelRC.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
		modelRC.RegisterSpace = 0;
		modelRC.ShaderRegister = 0;
		rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[0].Constants = modelRC;
		rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS normalRC{ 0 };
		normalRC.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
		normalRC.RegisterSpace = 0;
		normalRC.ShaderRegister = 1;
		rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[1].Constants = normalRC;
		rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS viewProjRC{ 0 };
		viewProjRC.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
		viewProjRC.RegisterSpace = 0;
		viewProjRC.ShaderRegister = 2;
		rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[2].Constants = viewProjRC;
		rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS materialRC{ 0 };
		materialRC.Num32BitValues = sizeof(Material) / 4;
		materialRC.RegisterSpace = 0;
		materialRC.ShaderRegister = 3;
		rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[3].Constants = materialRC;
		rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_CONSTANTS camPosRC{ 0 };
		camPosRC.Num32BitValues = sizeof(DirectX::XMVECTOR) / 4;
		camPosRC.RegisterSpace = 0;
		camPosRC.ShaderRegister = 4;
		rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParams[4].Constants = camPosRC;
		rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_DESCRIPTOR_RANGE* descriptorTableRange = new D3D12_DESCRIPTOR_RANGE;
		descriptorTableRange->RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorTableRange->NumDescriptors = 2;
		descriptorTableRange->BaseShaderRegister = 0;
		descriptorTableRange->RegisterSpace = 0;
		descriptorTableRange->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
		descriptorTable.NumDescriptorRanges = 1;
		descriptorTable.pDescriptorRanges = descriptorTableRange;
		rootParams[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParams[5].DescriptorTable = descriptorTable;
		rootParams[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		return rootParams;
	}

	void DrawExecutor::CreateRootSig(std::vector<D3D12_ROOT_PARAMETER> rootParams)
	{
		ID3DBlob* rootBlob;
		ID3DBlob* errorBlob;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC rootDesc{0};
		rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootDesc.NumParameters = rootParams.size();
		rootDesc.pParameters = rootParams.data();
		rootDesc.pStaticSamplers = &sampler;
		rootDesc.NumStaticSamplers = 1;
		D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob);
		m_graphics.GetDevice()->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
		rootBlob->Release();
	}

	void DrawExecutor::CreatePSO(DrawExecutor::Shaders shaders)
	{
		auto path = std::filesystem::current_path();
		path /= shaders.vertexShader;
		std::ifstream compiledShader;
		compiledShader.open(path.c_str(), std::ifstream::in | std::ifstream::binary);
		auto a = compiledShader.is_open();
		char* VSbuffer;
		int length;
		compiledShader.seekg(0, std::ios::end);
		length = compiledShader.tellg();
		compiledShader.seekg(0, std::ios::beg);
		VSbuffer = new char[length];
		compiledShader.read(VSbuffer, length);
		compiledShader.close();
		D3D12_SHADER_BYTECODE VSbytecode{ 0 };
		VSbytecode.pShaderBytecode = VSbuffer;
		VSbytecode.BytecodeLength = length;
		path = std::filesystem::current_path();
		path /= shaders.pixelShader;
		compiledShader.open(path.c_str(), std::ifstream::in | std::ifstream::binary);
		char* PSbuffer;
		compiledShader.seekg(0, std::ios::end);
		length = compiledShader.tellg();
		compiledShader.seekg(0, std::ios::beg);
		PSbuffer = new char[length];
		compiledShader.read(PSbuffer, length);
		compiledShader.close();
		D3D12_SHADER_BYTECODE PSbytecode{ 0 };
		PSbytecode.pShaderBytecode = PSbuffer;
		PSbytecode.BytecodeLength = length;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{ 0 };
		psoDesc.VS = VSbytecode;
		psoDesc.PS = PSbytecode;
		psoDesc.pRootSignature = m_rootSignature;
		psoDesc.InputLayout = SimpleVertex::GetLayout();
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.SampleDesc = { 1,0 };
		psoDesc.SampleMask = 0xffffffff;
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		m_graphics.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
	}
}