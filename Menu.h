#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"

#include "Sprite.h"

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
	void SetDXInput(DXInput* dxInput) { this->dxInput = dxInput; }
	int GetSerect() { return serect; }
	int GetIsSerect() { return isSerect; }
	void Reset();

private:
	//�L�[�{�[�h
	Input* input;
	//�R���g���[���[
	DXInput* dxInput;

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

};