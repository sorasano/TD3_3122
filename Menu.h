#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"

#include "Sprite.h"

enum MenuSerect {
	RESTART,
	TITLE,
};

class Menu
{
private: // �G�C���A�X
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(XMFLOAT2 position);

private:
	//�L�[�{�[�h
	static Input* input;
	//�R���g���[���[
	static DXInput* dxInput;

	XMFLOAT2 position = {};

	//�x�[�X�̉摜
	Sprite* baseSprite;
	//"�`�F�b�N�|�C���g�ɖ߂�"
	Sprite* restartSprite;
	//"�^�C�g���ɖ߂�"
	Sprite* titleSprite;

	//���j���[���J���Ă��邩
	bool isMenu;

	//����I�����Ă��邩
	int serect;

};

