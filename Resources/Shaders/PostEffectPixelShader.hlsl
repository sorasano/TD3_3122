#include "PostEffectHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	////画像のデータ
	//float4 texData = float4(tex0.Sample(smp,input.uv));

	////アルファ値を合成
	//return float4(texData.x * color.x, texData.y * color.y, texData.z * color.z, texData.w * color.w);

	float4 colortex0 = tex0.Sample(smp,input.uv);
	float4 colortex1 = tex1.Sample(smp,input.uv);

	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = colortex1;
	}

	/*if (colortex0.w != 0)
	{
		colortex0.x = 1;
		colortex0.y = 0;
		colortex0.z = 1;
	}*/

	return colortex0;
}