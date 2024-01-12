// #include "ShaderOps.hlsl"
float3 MapNormal(
	  const in float3 tan
	, const in float3 bitan
	, const in float3 normal
	, const in float2 tc
	, uniform Texture2D nmap
	, uniform SamplerState splr)
{
	// Build the transform (rotation) into same space as tan/bitan/normal (target space)
    const float3x3 tanToTarget = float3x3(
		normalize(tan)
		, normalize(bitan)
		, normalize(normal)
	);
	// Sample and unpack the normal from texture into tangent space   
    const float3 normalSample = nmap.Sample(splr, tc).xyz;
    const float3 tanNormal = normalSample * 2.0f - 1.0f;
	// bring normal from tanspace into target space
    return normalize(mul(tanNormal, tanToTarget));
}

float Attenuate(uniform float attConst, uniform float attLin, uniform float attQuad, const in float distFragToL)
{
    return 1.0f / (attConst + attLin * distFragToL, +attQuad * (distFragToL * distFragToL));
}

float3 Diffuse(
	uniform float3 diffuseColor
	, uniform float diffuseIntensity
	, const in float att
	, const in float3 viewDirFragToL
	, const in float3 viewNormal)
{
    return diffuseColor * diffuseIntensity * att * max(0.0f, dot(viewDirFragToL, viewNormal));
}

float3 Speculate(
	const in float3 specularColor
	, uniform float specularIntensity
	, const in float3 viewNormal
	, const in float3 viewFragToL
	, const in float3 viewPos
	, const in float att
	, const in float specularPower)
{
	// calculate reflected light vector
    const float3 w = viewNormal * dot(viewFragToL, viewNormal);
    const float3 r = normalize(w * 2.0f - viewFragToL);
	// vector from camera to fragment (in view space)
    const float3 viewCamToFrag = normalize(viewPos);
	// calculate specular component color based on angle between
	// viewing vector and reflection vector, narrow with power function
    return att * specularColor * specularIntensity * pow(max(0.0f, dot(-r, viewCamToFrag)), specularPower);
}

// #include "LightVectorData.hlsl"
struct LightVectorData
{
    float3 vToL;
    float3 dirToL;
    float distToL;
};

LightVectorData CalculateLightVectorData(const in float3 lightPos, const in float3 fragPos)
{
    LightVectorData lv;
    lv.vToL = lightPos - fragPos;
    lv.distToL = length(lv.vToL);
    lv.dirToL = lv.vToL / lv.distToL;
    return lv;
}
// #include "PointLight.hlsl"
cbuffer LightCBuf
{
    float3 viewLightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

#include "Transform.hlsl"

Texture2D tex;
Texture2D nmap : register(t2);

SamplerState splr;


float4 main(float3 viewFragPos : Position, float3 viewNormal : Normal, float2 tc : Texcoord) : SV_Target
{
	// Sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
		// Sample and unpack the normal data
        const float3 normalSample = nmap.Sample(splr, tc).xyz;
        const float3 objectNormal = normalSample * 2.0f - 1.0f;
        // Bring normal from object space into view space
        viewNormal = normalize(mul(objectNormal, (float3x3) modelView));
    }
	// Fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);
	// Attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);
	// Diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);
	// specular
    const float3 specular = Speculate(
		specularIntensity.rrr
		, 1.0f
		, viewNormal
		, lv.vToL
		, viewFragPos
		, att
		, specularPower
	);
	// Final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}