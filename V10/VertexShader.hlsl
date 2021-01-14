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
	float3 tangent: TANGENT;
};

struct VertexShaderOutput
{
	float4 positionSV: SV_POSITION;
	float4 position: POSITION;
	float2 texCoord: TEXCOORD;
	float3 normal: NORMAL;
	float3x3 TBN : TBN_MATRIX;
};

VertexShaderOutput main(VertexShaderInput IN)
{
	float3 T = normalize(float3(mul(ModelCB.model, float4(IN.tangent, 0.0)).xyz));
	float3 N = normalize(float3(mul(ModelCB.model, float4(IN.normal, 0.0)).xyz));
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);

	VertexShaderOutput OUT;
	OUT.texCoord = IN.texCoord;
	OUT.normal = IN.normal;
	OUT.position = mul(ModelCB.model, float4(IN.position, 1.0f));
	OUT.positionSV = mul(vpCB.vp, OUT.position);
	OUT.TBN = TBN;

	return OUT;
}