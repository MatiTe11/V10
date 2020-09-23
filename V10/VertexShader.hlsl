// simple vertex shader
struct ModelViewProjection
{
	matrix MVP;
};

ConstantBuffer<ModelViewProjection> ModelViewProjectionCB : register(b0);

struct VertexPosColor
{
	float3 pos : POSITION;
	float3 col : COLOR;
};

struct VertexShaderOutput
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VertexShaderOutput main(VertexPosColor IN)
{
	VertexShaderOutput OUT;
	OUT.color = float4(IN.col, 1.0f);
	//OUT.position = float4(IN.pos, 1.0f);
	OUT.position = mul(ModelViewProjectionCB.MVP, float4(IN.pos, 1.0f));

	return OUT;
}