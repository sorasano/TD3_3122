#include "FbxShadowHeader.hlsli"

//�X�L�j���O��̒��_�@��������
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

//�X�L�j���O�v�Z
SkinOutput ComputeSkin(VSInput input)
{
	//�[���N���A
	SkinOutput output = (SkinOutput)0;

	uint iBone;	//�v�Z����{�[���ԍ�
	float weight;	//�{�[���E�F�C�g
	matrix m;	//�X�L�j���O�s��	

	//�{�[��0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	//�{�[��3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	return output;
}

//�e�p�s�N�Z���V�F�[�_�[�ɓn���l
//struct VSOutput2
//{
//	float4 pos : POSITION;	//�ˉe�ϊ����W
//	float4 zCalcTex : TEXCOORD;	//Z�l�Z�o�p�e�N�X�`��
//	float4 col : COLOR0;	//�f�B�t���[�Y�F
//};

//�G���g���[�|�C���g
VSOutput main(VSInput input)
{
	//�X�L�j���O�v�Z
	SkinOutput skinned = ComputeSkin(input);
	//�@���Ƀ��[���h�s��ɂ��X�P�[�����O ��]��K�p
	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
	float4 wpos = mul(world, skinned.pos);
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	//�s��ɂ����W�Ԋ�
	output.svpos = mul(mul(viewproj, world), skinned.pos);
	//���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	//���͒l�����̂܂܎��̃X�e�[�W�n��
	output.uv = input.uv;
	//���[���h���W
	output.worldpos = wpos;

	//�e�����ꍇ
	if (shadows[0].active == true)
	{
		////�J�����̖ڐ��ɂ�郏�[���h�r���[�ϊ�
		//float4x4 mat;
		//mat = mul(world, view);
		//mat = mul(mat, viewproj);
		//output.pos = mul(input.pos, mat);

		////���C�g�̖ڐ��ɂ��r���[�ϊ�

		//mat = mul(world, shadows[0].lightView);
		//mat = mul(mat, shadows[0].lightViewProj);
		//output.zCalcTex = mul(input.pos, mat);

		////�@���ƃ��C�g�̕������璸�_�̐F������
		//float3 N = normalize(mul(input.normal, world));
		//float3 LightDirect = normalize(float3(shadows[0].lightView._13,
		//	shadows[0].lightView._23, shadows[0].lightView._33));
		//output.col = float4(0.0, 0.6, 1.0, 1.0) * (0.3 + dot(N, -LightDirect) * (1 - 0.3f));

	}

	return output;
}