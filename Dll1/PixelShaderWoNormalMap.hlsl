struct Material
{
    float ambient;
    float diffuse;
    float specular;
};

struct CamPos
{
    vector camPos;
};

ConstantBuffer<Material> MaterialCB : register(b2);
ConstantBuffer<CamPos> CamPosCB : register(b3);

Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

struct VS_OUTPUT
{
    float4 positionSV: SV_POSITION;
    float4 position: POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal: NORMAL;
    float3x3 TBN :TBN_MATRIX;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    //diffuse
    float3 lightDir = (-input.position);
    lightDir = normalize(lightDir);
    float diff = max(dot(input.normal, lightDir), 0.0);
    //specular
    float3 viewDir = normalize(CamPosCB.camPos - input.position);
    float3 reflectDir = reflect(-lightDir, input.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);


    return (t1.Sample(s1, input.texCoord) * (MaterialCB.ambient + diff + spec));
}