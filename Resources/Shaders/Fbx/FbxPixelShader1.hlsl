#include "FbxHeader1.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
texture2D<float> tex1 : register(t1);	//�J��������̐[�x
texture2D<float> tex2 : register(t2);	//���C�g����̐[�x
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

struct PSOutput 
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;
	//�e�N�X�`���}�b�s���O
	float4 texcoord = tex.Sample(smp,input.uv);
	//Lambert����
	float3 light = normalize(float3(1,-1,1));	//�E���� �����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//���C�g���猩��UV�l
	float3 posFromLightVP = input.tpos.xyz / input.tpos.w;
	float2 shadowUV = (posFromLightVP + float2(1, -1)) * float2(0.5, -0.5);
	////�e�e�N�X�`��
	//float depthFromLight = tex2.Sample(smp, shadowUV);
	//float shadowWeight = 1.0f;
	//if (depthFromLight < posFromLightVP.z - 0.005)
	//{
	//	shadowWeight = 0.2f;
	//}


	////���_���王�_�ւ̃x�N�g��
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	////����
	//float3 shininess = 4.0f;

	////�_����
	//for (int i = 0; i < pointLightNum; i++)
	//{
	//	if (pointLights[i].active)
	//	{
	//		//���C�g�̃x�N�g��
	//		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
	//		//�x�N�g���̒���
	//		float d = length(lightv);
	//		//���K�����A�P�ʃx�N�g���ɂ���
	//		lightv = normalize(lightv);
	//		//���������W��
	//		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
	//			pointLights[i].lightatten.z * d * d);
	//		//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//���ˌ��x�N�g��
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//�g�U���ˌ�
	//		//float3 diffuse = dotlightnormal * m_diffuse;
	//		float diffuse1 = saturate(dot(-light, input.normal));
	//		//���ʔ��ˌ�
	//		/*float3 specular = pow(saturate(dot(reflect,eyedir)),shininess)*/
	//		shadecolor.rgb *= atten/* * (diffuse1)*pointLights[i].lightcolor*/;
	//	}
	//}

	//for (int i = 0; i < spotLightNum; i++)
	//{
	//	if (spotLights[i].active)
	//	{
	//		//���C�g�ւ̕����x�N�g��
	//		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
	//		//�x�N�g���̒���
	//		float d = length(lightv);
	//		//���K�����A�P�ʃx�N�g���ɂ���
	//		lightv = normalize(lightv);
	//		//���������W��
	//		float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
	//		// �p�x����
	//		float cos = dot(lightv, spotLights[i].lightv);
	//		//�����J�n�p�x����A �����I���p�x�ɂ����Č���
	//		// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
	//		float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
	//		// �p�x��������Z
	//		atten *= angleatten;
	//		//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//���ˌ��x�N�g��
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//�g�U���ˌ�
	//		float diffuse1 = saturate(dot(-light, input.normal));
	//		// ���ʔ��ˌ�
	//		/*float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;*/
	//		//�S�ĉ��Z����
	//		shadecolor.rgb *= atten/* * (diffuse + specular) * spotLights[i].lightcolor*/;
	//	}
	//}

	output.target0 = shadecolor * texcoord;
	//�e������
	output.target1 = float4(1, 1, 1, 1);
	return output;
}