//�}�e���A��
cbuffer ConstBuff : register(b0)
{
	float4 color;

	/*float3 status;*/
	////�t�H�[�J�X
	//float focus;
	////F�l
	//float fnumber;
	////����
	//float strength;

	float2 window;
};

//�ό`�s��
cbuffer ConstBuffTransform : register(b1)
{
	matrix mat;
};

//�t�H�[�J�X�Ƃ�
cbuffer ConstBuffStatus : register(b2)
{
	//�t�H�[�J�X
	float focus;
	//f�l
	float fnumber;
	//����
	float strength;
};

//���_�V�F�[�_�̏o�͍\����
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv
	float2 uv : TEXCOORD;
};