#pragma once
#include "DirectXMath.h"
#include "Input.h"

#include "Sprite.h"
#include "AudioManager.h"

enum MenuSerect {
	MENURESET,
	MENUTITLE,
	MENUCLOSE
};

class Menu
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	void Initialize(Input* input);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(XMFLOAT2 position);
	void SetInput(Input* input) { this->input = input; }
	int GetSerect() { return serect; }
	int GetIsSerect() { return isSerect; }
	void Reset();

private:
	//�L�[�{�[�h
	Input* input;

	XMFLOAT2 position = {};

	//�x�[�X�̉摜
	Sprite* baseSprite;
	//"�`�F�b�N�|�C���g�ɖ߂�"
	Sprite* restartSprite;
	//"�^�C�g���ɖ߂�"
	Sprite* titleSprite;
	//"���j���[�����"
	Sprite* closeSprite;

	//����I�����Ă��邩
	int serect;

	//�V�[���؂�ւ���������
	bool isSerect;

	//��
	//�I����
	AudioManager* selectSE;
	float selectSEVolume = 0.1f;

	//���艹
	AudioManager* pickSE;
	float pickSEVolume = 0.05f;
};