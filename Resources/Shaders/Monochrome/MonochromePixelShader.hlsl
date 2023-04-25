#include "MonochromeEffectHeader.hlsli"

texture2D<float4> tex0 : register(t0);
texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	////�摜�̃f�[�^
	//float4 texData = float4(tex0.Sample(smp,input.uv));

	////�A���t�@�l������
	//return float4(texData.x * color.x, texData.y * color.y, texData.z * color.z, texData.w * color.w);

	float4 colortex0 = tex0.Sample(smp,input.uv);
	float4 colortex1 = tex1.Sample(smp,input.uv);

	//�A���t�@�l��0�łȂ���Ύw��̐F�ɕύX
	if (colortex0.w != 0)
	{
		colortex0.x = color.x;
		colortex0.y = color.y;
		colortex0.z = color.z;
	}

	return colortex0;
}