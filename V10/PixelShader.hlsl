struct Material
{
    float ambient;
    float diffuse;
    float specular;
};

ConstantBuffer<Material> MaterialCB : register(b1);

Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct VS_OUTPUT
{
    float4 positionSV: SV_POSITION;
    float3 position: POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal: NORMAL;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float3 lightDir = ( (input.position).xyz - float3(0, 0, 0));
    lightDir = normalize(lightDir);
    float diff = max(dot(input.normal, lightDir), 0.0);

    return (t1.Sample(s1, input.texCoord) * (MaterialCB.ambient + diff));
}