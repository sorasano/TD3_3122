#pragma once

#include "FbxModel.h"
#include "Camera.h"

#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include "string.h"
#include "Light.h"
#include "LightGroup.h"

class FbxObject3D
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�萔
	static const int MAX_BONES = 320;
	//�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		XMMATRIX lightviewproj;
		float alpha;
	};
	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:	//�ÓI�����o�֐�
	//�Z�b�^�[
	static void SetDevice(ID3D12Device* device) { FbxObject3D::device = device; }
	static void SetCamera(Camera* camera) { FbxObject3D::camera = camera; }
	static void SetLight(Light* light) { FbxObject3D::light = light; }
	static void SetLightGroup(LightGroup* lightGroup) { FbxObject3D::lightGroup = lightGroup; }

private://�ÓI�����o�ϐ�
	static ID3D12Device* device;
	static Camera* camera;
	static Light* light;
	static LightGroup* lightGroup;

public://�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawStandard(ID3D12GraphicsCommandList* cmdList);	//�e�Ȃ��`�������ꍇ
	//���f���̃Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }
	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipelineLightView();
	static void CreateGraphicsPipeline0();
	static void CreateGraphicsPipeline1();
	//�A�j���[�V�����J�n
	void PlayAnimation();
	//�A�j���[�V������~
	void StopAnimation();
	//�A�j���[�V�����ĊJ
	void RestartAnimation();

	//�Z�b�^�[
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }
	void SetScale(XMFLOAT3 sca) { scale = sca; }
	void SetSRV(ID3D12DescriptorHeap* SRV) { depthSRV = SRV; }
	void SetAlpha(float f) { alpha = f; }

private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffTransform;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature0;
	static ComPtr<ID3D12RootSignature>rootsignature1;
	static ComPtr<ID3D12RootSignature>rootsignature2;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate0;
	static ComPtr<ID3D12PipelineState>pipelinestate1;
	static ComPtr<ID3D12PipelineState>pipelinestate2;

private:
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J���s��
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	FbxModel* model = nullptr;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource>constBuffSkin;

	//�O������󂯎��SRV
	ID3D12DescriptorHeap* depthSRV;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

	float alpha = 1.0f;
};