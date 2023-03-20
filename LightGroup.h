#pragma once

#include "DirectXMath.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "vector"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "Shadow.h"

class LightGroup
{
private: // �G�C���A�X 
// Microsoft::WRL���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �萔
	static const int DirLightNum = 3;
	static const int PointLightNum = 3;
	static const int SpotLightNum = 3;
	static const int CircleShadowNum = 1;
	static const int ShadowNum = 1;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\���� 
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�X�|�b�g���C�g
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//�e�p
		Shadow::ConstBufferData shadows[ShadowNum];
		//�ۉe�p
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};

private:	//�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;

public: // �ÓI�����o�֐� 

	//�ÓI������
	static void StaticInitialize(ID3D12Device* device);
	//�C���X�^���X�擾
	static LightGroup* Create();

public:	//�����o�֐�

	// �萔�o�b�t�@�]��
	void TransferConstBuffer();
	//�����̃��C�g���Z�b�g
	void SetAmbientColor(const XMFLOAT3& color);
	//���s�����̗L���t���O���Z�b�g
	void SetDirLightActive(int index, bool active);
	//���s�����̃��C�g�������Z�b�g
	void SetDirLightDir(int index, const XMVECTOR& lightdir);
	//���s�����̃��C�g�F���Z�b�g
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);
	//�W���̃��C�g�ݒ�
	void DefaultLightSetting();

	//�_�����̗L���t���O���Z�b�g
	void SetPointLightActive(int index, bool active);
	//�_�����̃��C�g���W���Z�b�g
	void SetPointLightPos(int index, const XMFLOAT3& lightpos);
	//�_�����̃��C�g�F���Z�b�g
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);
	//�_�����̃��C�g���������W�����Z�b�g
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);

	//�X�|�b�g���C�g�̗L���t���O���Z�b�g
	void SetSpotLightActive(int index, bool active);
	//�X�|�b�g���C�g�̃��C�g�������Z�b�g
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);
	//�X�|�b�g���C�g�̃��C�g���W���Z�b�g
	void SetSpotLightPos(int index, const XMFLOAT3& lightpos);
	//�X�|�b�g���C�g�̃��C�g�F���Z�b�g
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);
	//�X�|�b�g���C�g�̃��C�g�����W�����Z�b�g
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);
	//�X�|�b�g���C�g�̃��C�g�����p�x���Z�b�g
	void SetSpotLightFactorAngle(int index, const XMFLOAT2& lightFactorAngle);

	//�ۉe�̗L���t���O�Z�b�g
	void SetCircleShadowActive(int index, bool active);
	//�ۉe�̃L���X�^�[���W�Z�b�g
	void SetCircleShadowCasterPos(int index, const XMFLOAT3& casterPos); 
	//�ۉe�̕����Z�b�g
	void SetCircleShadowDir(int index, const XMVECTOR& lightdir);
	//�ۉe�̃L���X�^�[�ƃ��C�g�̋����Z�b�g
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight); 
	//�ۉe�̌����W���Z�b�g
	void SetCircleShadowAtten(int index, const XMFLOAT3& lightAtten);
	//�ۉe�̌����p�x�Z�b�g
	void SetCircleShadowFactorAngle(int index, const XMFLOAT2& lightFactorAngle);

	//�e�̗L���t���O�Z�b�g
	void SetShadowActive(int index, bool active);
	//�e�̃��C�g���W�Z�b�g
	void SetShadowLightPos(int index, XMFLOAT3 lightPos, XMFLOAT3 target, XMFLOAT3 up);
	//�e�̃��C�g�r���[���W
	void SetShadowLightView(int index, XMMATRIX lightView);
	//�e�̎ˉe�ϊ�
	void SetShadowLightViewProj(int index, XMMATRIX lightViewProj);

	void Initialize();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

private: // �����o�ϐ� 
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff; 
	// �����̐F
	XMFLOAT3 ambientColor = { 1,1,1 };
	// ���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	//�_�[�e�B�t���O
	bool dirty = false;
	//�_�����̔z��
	PointLight pointLights[PointLightNum];
	//�X�|�b�g���C�g�̔z��
	SpotLight spotLights[SpotLightNum];
	//�ۉe�̔z��
	CircleShadow circleShadows[CircleShadowNum];
	//�e�̔z��
	Shadow shadows[ShadowNum];
};

