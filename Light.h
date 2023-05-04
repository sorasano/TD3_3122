#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
#include "FbxModel.h"

using namespace DirectX;

class Light
{
public:
	//�V���O���g���C���X�^���X
	static Light* GetInstance();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�Q�b�^�[�Z�b�^�[
	void SetTarget(XMFLOAT3 pos) {target_ = pos;}
	void SetEye(XMFLOAT3 pos) { eye_ = pos; }
	void SetColor(XMFLOAT4 col) { color = col; }
	void SetDir(XMFLOAT3 d) { dir = d; }
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMFLOAT3 GetDir() { return dir; };
	XMFLOAT4 GetCOlor() { return color; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };
private:
	//�ˉe�ϊ�
	XMMATRIX matProjection_;
	//�r���[�ϊ��s��
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 10, 10 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };
	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT3 dir = { 1,-1,-1 };
};

