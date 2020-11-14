struct ModelViewProjection
{
	matrix MVP;
};

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);

struct VertexShaderInput
{
	float3 position: POSITION;
	float2 texCoord: TEXCOORD;
};

struct VertexShaderOutput
{
	float4 position: SV_POSITION;
	float2 texCoord: TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput IN)
{
	VertexShaderOutput OUT;
	OUT.texCoord = IN.texCoord;
	OUT.position = mul(ModelViewProjectionCB.MVP, float4(IN.position, 1.0f));

	return OUT;
}