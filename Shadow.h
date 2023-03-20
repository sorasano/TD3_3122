#pragma once
#include "DirectXMath.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "vector"

class Shadow
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

public://�T�u�N���X

//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		unsigned int active;
		XMMATRIX lightView;	//���C�g�̃r���[���W
		XMMATRIX lightViewProj;	//���C�g�̎ˉe�ϊ��s��
	};

public: //�����o�֐�
	void SetLightPos(const XMFLOAT3& lightPos, const XMFLOAT3& target, const XMFLOAT3& up);	//���C�g�̍��W���擾���ăr���[�s��v�Z
	inline const XMFLOAT3& GetLightPos() { return lightPos; }
	inline void SetLightView(const XMMATRIX& lightView) { this->lightView = lightView; }
	inline const XMMATRIX& GetLightView() { return lightView; }
	inline void SetLightViewProj(const XMMATRIX& lightViewProj) { this->lightViewProj = lightViewProj; }
	inline const XMMATRIX& GetLightViewProj() { return lightViewProj; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }


private:	//�����o�ϐ�

	//// ���� (�P�ʃx�N�g��)
	//XMVECTOR dir = { 1,0,0,0 };
	////�L���X�^�[�ƃ��C�g�̋���
	//float distanceCasterLight = 100.0f;
	//// �L���X�^�[���W(���[���h���W�n)
	//XMFLOAT3 casterPos = { 0,0,0 };
	//// ���������W��
	//XMFLOAT3 atten = { 0.5f, 0.6f, 0.0f };
	//// �����p�x
	//XMFLOAT2 factorAngleCos = { 0.2f, 0.5f };
	
	XMFLOAT3 lightPos;	//���C�g�̍��W
	XMMATRIX lightView;	//���C�g�̃r���[���W
	XMMATRIX lightViewProj;	//���C�g�̎ˉe�ϊ��s��
	// �L���t���O
	bool active = false;

};



