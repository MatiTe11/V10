#include "stdafx.h"
#include "Drawable.h"





Drawable::Drawable(Graphics * graphics)
	:m_graphics(graphics)
{
	CreateRootSig();
	CreatePSO();
	m_mesh = new Mesh(graphics);
}

Drawable::~Drawable()
{
}

void Drawable::AddToCL(ID3D12GraphicsCommandList * cl)
{
	cl->SetPipelineState(m_pso);
	cl->SetGraphicsRootSignature(m_rootSignature);
	DirectX::XMMATRIX mvpMat = DirectX::XMMatrixMultiply(m_modelMat, m_viewMat);
	mvpMat = DirectX::XMMatrixMultiply(mvpMat, m_projectionMat);
	cl->SetGraphicsRoot32BitConstants(0, sizeof(DirectX::XMMATRIX) / 4, &mvpMat, 0);
	
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

void Drawable::Draw()
{
}

void Drawable::CreateRootSig()
{
	ID3DBlob * rootBlob;
	ID3DBlob * errorBlob;

	D3D12_ROOT_PARAMETER rootParam{ NULL };
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	D3D12_ROOT_CONSTANTS rc{ 0 };
	rc.Num32BitValues = sizeof(DirectX::XMMATRIX) / 4;
	rc.RegisterSpace = 0;
	rc.ShaderRegister = 0;
	rootParam.Constants = rc;
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rootDesc{ 0 };
	rootDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootDesc.NumParameters = 1;
	rootDesc.pParameters = &rootParam;
	D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob);
	m_graphics->GetDevice()->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	rootBlob->Release();
}

void Drawable::CreatePSO()
{
	std::ifstream compiledShader;
	compiledShader.open("C:\\Users\\matit\\source\\repos\\V10\\x64\\Debug\\VertexShader.cso", std::ifstream::in | std::ifstream::binary);
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
	compiledShader.open("C:\\Users\\matit\\source\\repos\\V10\\x64\\Debug\\PixelShader.cso", std::ifstream::in | std::ifstream::binary);
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
	D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc;
	inputLayoutDesc.NumElements = 2;
	D3D12_INPUT_ELEMENT_DESC inputElement[] = { { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
											    { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } };
	inputLayoutDesc.pInputElementDescs = inputElement;
	psoDesc.InputLayout = inputLayoutDesc;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = 0xffffffff;
	m_graphics->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso));
}
