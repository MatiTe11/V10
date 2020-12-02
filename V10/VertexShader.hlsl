struct ModelMatrix
{
	matrix model;
};

struct ViewProjectionMatrix
{
	matrix vp;
};
ConstantBuffer<ModelMatrix> ModelCB : register(b0);
ConstantBuffer<ViewProjectionMatrix> vpCB : register(b1);

struct VertexShaderInput
{
	float3 position: POSITION;
	float2 texCoord: TEXCOORD;
	float3 normal: NORMAL;
};

struct VertexShaderOutput
{
	float4 positionSV: SV_POSITION;
	float4 position: POSITION;
	float2 texCoord: TEXCOORD;
	float3 normal: NORMAL;
};

VertexShaderOutput main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.texCoord = IN.texCoord;
	OUT.normal = IN.normal;
	OUT.position = mul(ModelCB.model, float4(IN.position, 1.0f));
	OUT.positionSV = mul(vpCB.vp, OUT.position);

	return OUT;
}