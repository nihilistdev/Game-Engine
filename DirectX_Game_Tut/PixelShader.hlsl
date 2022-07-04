Texture2D EarthColor : register(t0);
sampler EarthColorSampler: register(s0);

Texture2D EarthSpecular : register(t1);
sampler EarthSpecularSampler: register(s1);

Texture2D CloudsTexture : register(t2);
sampler CloudsSampler: register(s2);

Texture2D EarthNightTexture : register(t3);
sampler EarthNightSampler: register(s3);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 direction_to_camera: TEXCOORD2;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
	float4 m_camera_direction;
	float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 earth_color = EarthColor.Sample(EarthColorSampler, 1.0 - input.texcoord);
	float earth_spec = EarthSpecular.Sample(EarthSpecularSampler, 1.0 - input.texcoord).r;
	float clouds = CloudsTexture.Sample(CloudsSampler, 1.0 - input.texcoord + float2(m_time / 100.0f, 0)).r;
	float4 earth_night = EarthNightTexture.Sample(EarthNightSampler, 1.0 - input.texcoord);
	float ka = 1.5;
	float3 ia = float3(0.09, 0.082, 0.082);
	ia *= (earth_color.rgb + clouds);
	float3 ambient_light = ka * ia;
	float kd = 0.7;

	float ammount_difuse_light = dot(m_light_direction.xyz, input.normal);

	float3 id_day = float3(1.0, 1.0, 1.0);
	id_day *= (earth_color.rgb + clouds);

	float3 id_night = float3(1.0, 1.0, 1.0);
	id_night *= (earth_night.rgb + clouds);

	float3 id = lerp(id_night, id_day, (ammount_difuse_light + 1.0) / 2.0);

	float3 diffuse_light = kd * id;

	float ks = earth_spec;
	float3 is = float3(1.0, 1.0, 1.0);
	float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
	float shiness = 30.0;
	float ammount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shiness);
	float3 specular_light = ks * ammount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;
	return float4(final_light, 1.0);
	//return Texture.Sample(TextureSampler, input.texcoord * 0.5);
}