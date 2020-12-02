struct Material
{
    float ambient;
    float diffuse;
    float specular;
};

ConstantBuffer<Material> MaterialCB : register(b2);

Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct VS_OUTPUT
{
    float4 positionSV: SV_POSITION;
    float4 position: POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal: NORMAL;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float3 lightDir = (-input.position);
    lightDir = normalize(lightDir);
    float diff = max(dot(input.normal, lightDir), 0.0);

    return (t1.Sample(s1, input.texCoord) * (MaterialCB.ambient + diff));
}