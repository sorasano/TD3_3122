#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"
#include"CubeModel.h"
#include"CubeObject3D.h"
#include "AudioManager.h"

class PushBlock
{
public://�����o�֐�
	//������
	void Initialize(FbxModel* blockModel, Player* player, CubeObject3D* cubeObject);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	void Reset();
	//�����蔻��
	void Collision();
	//�u���b�N������
	void Push();
	//�����Ȃ�
	void NoPush();
	//�����߂�����
	void pushback(CubeObject3D* cubeObject);

	//�Z�b�^�[
	void SetPosition(XMFLOAT3 position) { this->position = position; savepos = position; }

	void SetPositionX(float positionX) { this->position.x = positionX; }

	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	void Setrotate(XMFLOAT3 rotate) { this->rotate = rotate; }
	void SetSRV(ID3D12DescriptorHeap* SRV);

	//�Q�b�^�[
	bool GetIsPush() { return isPush; }
	bool GetNoPush() { return noPush; }
	XMFLOAT3 GetPosition() { return position; }
	CubeObject3D* GetCubeObject() { return cubeObject; }

	void StopSound();

public://�ÓI�����o�֐�
	static void SetInput(Input* input) { PushBlock::input = input; }

private://�����o�ϐ�
	XMFLOAT3 position = { 0.0f,1.0f,-1.0f };
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };
	//���ɓ���������
	XMFLOAT3 newposition = { 0.0f,1.0f,-1.0f };

	//����
	////�L�[�{�[�h
	static Input* input;
	//�u���b�N
	FbxObject3D* blockObject = nullptr;
	//�v���C���[
	Player* player;
	//����
	CubeObject3D* cubeObject = nullptr;
	bool isPush = false;
	bool noPush = false;
	//���Z�b�g�p
	XMFLOAT3 savepos = { 0.0f,0.0f,0.0f };

	//��
	AudioManager* pushSE;
	float pushSEVolume = 0.1f;


	bool isSound = false;
	bool reSound = false;
};