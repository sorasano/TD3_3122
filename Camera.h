#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
//#include "Player.h"
#define PI 3.14159265359

using namespace DirectX;

class Camera
{
public:
	static enum Mode
	{
		forward,
		right,
		backward,
		left,
	};
	static enum ApproachMode
	{
		Max,
		Min,
	};

public:
	//�V���O���g���C���X�^���X
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }
	//�C���X�g���N�^ �f�X�g���N�^
	Camera();
	~Camera();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�f�o�b�O Arrow�L�[�Ŏ��_���W�ύX
	void DebugUpdate();

	//�Q�b�^�[�Z�b�^�[
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };

private:
	//����
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;
	//�ˉe�ϊ�
	XMMATRIX matProjection_;
	//�r���[�ϊ��s��
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };

	//�^�[�Q�b�g�܂ł̋���
	float DebugTargetDistance = 20.0f;
	//���Z�p
	float DebugChangeRot = PI;
	float DebugChangeRot2 = 0.5f;
	float DebugChangeDistance = 0.0f;
};
