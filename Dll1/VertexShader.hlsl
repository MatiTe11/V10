struct ModelMatrix
{
	matrix model;
};
struct NormalMatrix
{
	float3x3 normal;
};
struct ViewProjectionMatrix
{
	matrix vp;
};
ConstantBuffer<ModelMatrix> ModelCB : register(b0);
ConstantBuffer<NormalMatrix> NormalCB : register(b1);
ConstantBuffer<ViewProjectionMatrix> vpCB : register(b2);

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
	//OUT.normal = mul(ModelCB.model, float4(IN.position, 1.0f));
	OUT.position = mul(ModelCB.model, float4(IN.position, 1.0f));
	//OUT.normal = mul(ModelCB.model, float4(IN.normal, 1.0f));
	OUT.normal = mul(NormalCB.normal, IN.normal);
	OUT.positionSV = mul(vpCB.vp, OUT.position);
	OUT.TBN = TBN;

	return OUT;
}