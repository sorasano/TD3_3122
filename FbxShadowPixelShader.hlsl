#include "FbxShadowHeader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//�V���h�E�}�b�v
Texture2D<float4> shadow_map : register(t1);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);
//1�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp1 : register(s1);
//�G���g���[�|�C���g
//float4 main(VSOutput input) : SV_TARGET
//{
//	//�e�N�X�`���}�b�s���O
//	float4 texcoord = tex.Sample(smp,input.uv);
//	//Lambert����
//	float3 light = normalize(float3(1,-1,1));	//�E���� �����̃��C�g
//	float diffuse = saturate(dot(-light, input.normal));
//	float brightness = diffuse + 0.3f;
//	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
//	//�A�e�ƃe�N�X�`���̐F������
//	return shadecolor * texcoord;
//}

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcoord = tex.Sample(smp,input.uv);
	//Lambert����
	float3 light = normalize(float3(1,-1,1));	//�E���� �����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	//for (int i = 0; i < DIR_LIGHT_NUM; i++) {
	//	if (dirLights[i].active) {
	//		//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
	//		// ���ˌ��x�N�g��
	//		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
	//		// �g�U���ˌ�
	//		float3 diffuse = dotlightnormal;
	//		//�S�ĉ��Z����
	//		shadecolor.rgb += (diffuse) * dirLights[i].lightcolor;
	//	}
	//}

	//for (int i = 0; i < POINTLIGHT_NUM; i++)
	//{
	//	if (pointLights[i].active)
	//	{
	//		//���C�g�ւ̃x�N�g��
	//		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
	//		//�x�N�g���̒���
	//		float d = length(lightv);
	//		//���K�����A�P�ʃx�N�g���ɂ���
	//		lightv = normalize(lightv);
	//		//���������W��
	//		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
	//		pointLights[i].lightatten.z * d * d);
	//		//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//���ˌ��x�N�g��
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		// �g�U���ˌ�
	//		float3 diffuse = dotlightnormal;
	//		//�S�ĉ��Z����
	//		shadecolor.rgb += atten * (diffuse) * pointLights[i].lightcolor;
	//	}
	//}

	//for (int i = 0; i < SPOTLIGHT_NUM; i++)
	//{
	//	if (spotLights[i].active)
	//	{
	//		//���C�g�ւ̕����x�N�g��
	//		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
	//		float d = length(lightv);
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
	//		// ���ˌ��x�N�g��
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//�g�U���ˌ�
	//		float3 diffuse = dotlightnormal;
	//		//�S�ĉ��Z����
	//		shadecolor.rgb += atten * (diffuse) * spotLights[i].lightcolor;
	//	}
	//}

	//for (int i = 0; i < CIRCLESHADOW_NUM; i++)
	//{
	//	if (circleShadows[i].active)
	//	{
	//		// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
	//		float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
	//		// ���e�����ł̋���
	//		float d = dot(casterv, circleShadows[i].dir);
	//		//���������W��
	//		float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
	//		// �������}�C�i�X�Ȃ�0�ɂ���
	//		atten *= step(0, d);
	//		// ���z���C�g�̍��W
	//		float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight; 
	//		// �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g�� (�P�ʃx�N�g��)
	//		float3 lightv = normalize(lightpos - input.worldpos.xyz);
	//		// �p�x����
	//		float cos = dot(lightv, circleShadows[i].dir);
	//		//�����J�n�p�x����A �����I���p�x�ɂ����Č���
	//		//�����J�n�p�x�̓�����1�{�����I���p�x�̊O����0�{�̋P�x
	//		float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos); 
	//		//�p�x��������Z
	//		atten *= angleatten;

	//		// �S�Č��Z���� 
	//		shadecolor.rgb -= atten;

	//	}
	//}

	float sm = shadow_map.Sample(smp1, input.pos.xy);
	float sma = (input.pos.z - 0.005f < sm) ? 1.0f : 0.5f;

	return tex.Sample(smp, input.uv) * sma;

	for (int i = 0; i < SHADOW_NUM; i++)
	{
		if (shadows[0].active)
		{
			// ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
			float ZValue = input.zCalcTex.z / input.zCalcTex.w;

			// �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
			float2 TransTexCoord;
			TransTexCoord.x = (1.0f + input.zCalcTex.x / input.zCalcTex.w) * 0.5f;
			TransTexCoord.y = (1.0f - input.zCalcTex.y / input.zCalcTex.w) * 0.5f;

			// ���A��Z�l���o
			float SM_Z = tex.Sample(smp, texcoord).x;

			// �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
			if (ZValue > SM_Z + 0.001f) {
				input.col.rgb = input.col.rgb * 0.5f;
			}
			return float4(1, 0, 1, 1);
			return (input.col * shadecolor);
		}
	}

	//�A�e�ƃe�N�X�`���̐F������
	return shadecolor * texcoord;
}
