#include "stdafx.h"
#include "Drawable.h"
#include "Texture2D.h"




Drawable::Drawable(Graphics * graphics)
	:m_graphics(graphics)
{
	CreateRootSig();
	CreatePSO();
	CreateDescHeap();
	m_mesh = new Mesh(graphics);
}

Drawable::~Drawable()
{
}

void Drawable::Draw(ID3D12GraphicsCommandList * cl)
{
	cl->SetPipelineState(m_pso);
	cl->SetGraphicsRootSignature(m_rootSignature);
	DirectX::XMMATRIX mvpMat = DirectX::XMMatrixMultiply(m_modelMat, m_viewMat);
	mvpMat = DirectX::XMMatrixMultiply(mvpMat, m_projectionMat);
	cl->SetGraphicsRoot32BitConstants(0, sizeof(DirectX::XMMATRIX) / 4, &mvpMat, 0);
	cl->SetDescriptorHeaps(1, &m_descHeap);
	cl->SetGraphicsRootDescriptorTable(1, m_descHeap->GetGPUDescriptorHandleForHeapStart());
	m_mesh->Draw(cl);
}

void Drawable::Update(float elapsedSeconds)
{
	const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 1);
	m_modelMat = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(elapsedSeconds));

	const DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
	const DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
	const DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
	m_viewMat = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	m_projectionMat = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), (16.0f/9.0f), 0.1f, 100.0f);
}

void Drawable::CreateRootSig()
{
	ID3DBlob * rootBlob;
	ID3DBlob * errorBlob;

	D3D12_ROOT_PARAMETER rootParam[2];

	D3D12_ROOT_CONSTANTS rc{ 0 };
	rc.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
	rc.RegisterSpace = 0;
	rc.ShaderRegister = 0;
	rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParam[0].Constants = rc;
	rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_DESCRIPTOR_RANGE  descriptorTableRanges[1]; // only one range right now
	descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // this is a range of shader resource views (descriptors)
	descriptorTableRanges[0].NumDescriptors = 1; // we only have one texture right now, so the range is only 1
	descriptorTableRanges[0].BaseShaderRegister = 0; // start index of the shader registers in the range
	descriptorTableRanges[0].RegisterSpace = 0; // space 0. can usually be zero
	descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
	descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges); 
	descriptorTable.pDescriptorRanges = &descriptorTableRanges[0];
	rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // this is a descriptor table
	rootParam[1].DescriptorTable = descriptorTable; // this is our descriptor table for this root parameter
	rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // our pixel shader will be the only shader accessing this parameter for now
	
	// create a static sampler
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_ROOT_SIGNATURE_DESC rootDesc{ 0 };
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootDesc.NumParameters = 2;
	rootDesc.pParameters = rootParam;
	rootDesc.pStaticSamplers = &sampler;
	rootDesc.NumStaticSamplers = 1;
	D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob);
	m_graphics->GetDevice()->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	rootBlob->Release();
}

void Drawable::CreatePSO()
{
	std::ifstream compiledShader;
	compiledShader.open("C:\\Users\\XPS\\V10\\x64\\Debug\\VertexShader.cso", std::ifstream::in | std::ifstream::binary);
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
	compiledShader.open("C:\\Users\\XPS\\V10\\x64\\Debug\\PixelShader.cso", std::ifstream::in | std::ifstream::binary);
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
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = 0xffffffff;
	m_graphics->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
}

void Drawable::CreateDescHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	m_graphics->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descHeap));

	Texture2D texture(m_graphics, 1080, 1080);
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture.GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	m_graphics->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, m_descHeap->GetCPUDescriptorHandleForHeapStart());
}
