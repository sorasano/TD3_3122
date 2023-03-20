//�{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	//�r���[�v���W�F�N�V�����s��
	matrix world;	//���[���h�s��
	float3 cameraPos;	//�J�������W
}

//cbuffer cbuff2 : register(b2)
//{
//	float3 lightv;	//���C�g�ւ̕����x�N�g��
//	float3 lightcolor;	//���C�g�̐F
//}

static const uint DIR_LIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;	//���C�g�ւ̕����x�N�g��
	float3 lightcolor;	//���C�g�̐F
	uint active;
};

//�_�����̐�
static const int POINTLIGHT_NUM = 3;

struct PointLight
{
	float3 lightpos;	//���C�g���W
	float3 lightcolor;	//���C�g�̐F
	float3 lightatten;	//���C�g���������W��
	uint active;
};

//�X�|�b�g���C�g�̐�
static const int SPOTLIGHT_NUM = 3;

struct SpotLight
{
	float3 lightv;		//���C�g�̌��������̋t�x�N�g��
	float3 lightpos;	//���C�g���W
	float3 lightcolor;	//���C�g�̐F
	float3 lightatten;	//���C�g���������W��
	float2 lightfactoranglecos;	//���C�g�����p�x�̃R�T�C��
	uint active;
};

//�ۉe�̐�
static const int CIRCLESHADOW_NUM = 2;

struct CircleShadow
{
	float3 dir;
	float3 casterPos;
	float distanceCasterLight;
	float3 atten;
	float2 factorAngleCos;
	uint active;
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIR_LIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

cbuffer skinning:register(b3)	//�{�[���̃X�L�j���O�s�񂪓���
{
	matrix matSkinning[MAX_BONES];
};

//�o�[�e�b�N�X�o�b�t�@�̓���
struct VSInput
{
	float4 pos : POSITION;	//�ʒu
	float3 normal : NORMAL;	//���_�@��
	float2 uv : TEXCOORD;	//�e�N�X�`���[���W
	uint4 boneIndices : BONEINDICES;	//�{�[���̔ԍ�
	float4 boneWeights : BONEWEIGHTS;	//�{�[���̃X�L���E�F�C�g
};

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;	//�V�X�e���p���_���W
	float4 worldpos : POSITION;	//�V�X�e���p���_���W
	float3 normal : NORMAL;	//�@��
	float2 uv : TEXCOORD;	//uv�l
};