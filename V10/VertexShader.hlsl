struct ModelViewProjection
{
	matrix MVP;
};

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);

struct VertexShaderInput
{
	float3 position: POSITION;
	float2 texCoord: TEXCOORD;
	float3 normal: NORMAL;
};

struct VertexShaderOutput
{
	float4 positionSV: SV_POSITION;
	float3 position: POSITION;
	float2 texCoord: TEXCOORD;
	float3 normal: NORMAL;
};

VertexShaderOutput main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.texCoord = IN.texCoord;
	OUT.normal = IN.normal;
	OUT.position = IN.position;
	OUT.positionSV = mul(ModelViewProjectionCB.MVP, float4(IN.position, 1.0f));

	return OUT;
}