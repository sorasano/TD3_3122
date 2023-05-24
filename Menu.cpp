#include "Menu.h"
Input* Menu::input = nullptr;
DXInput* Menu::dxInput = nullptr;

void Menu::Initialize()
{

	baseSprite = new Sprite();
	baseSprite->SetTextureNum(7);
	baseSprite->Initialize();
	baseSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	baseSprite->SetScale(XMFLOAT2(400, 400));
	baseSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	baseSprite->Update();

	restartSprite = new Sprite();
	restartSprite->SetTextureNum(8);
	restartSprite->Initialize();
	restartSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	restartSprite->SetScale(XMFLOAT2(400, 100));
	restartSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2));
	restartSprite->Update();

	titleSprite = new Sprite();
	titleSprite->SetTextureNum(9);
	titleSprite->Initialize();
	titleSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	titleSprite->SetScale(XMFLOAT2(400, 100));
	titleSprite->SetPosition(XMFLOAT2(window_width / 2, window_height / 2 + 100));
	titleSprite->Update();

	this->position = baseSprite->GetPosition();
}

void Menu::Update()
{

	//���j���[�J��
	if (input->PushKey(DIK_M)) {

		if (isMenu) {
			//���j���[���J��
			isMenu = false;
		}
		else {
			//���j���[�����
			isMenu = true;
			serect = RESTART;
		}
	}

	//���j���[����
	if (isMenu) {
		if (input->PushKey(DIK_W))
		{
			serect = RESTART;
		}
		else if (input->PushKey(DIK_S))
		{
			serect = TITLE;
		}


		//�I�𒆂�����
		if (serect == RESTART) {
			//restartSprite->color
		}
		else if (serect == TITLE) {

		}

	}

	this->position = baseSprite->GetPosition();

	baseSprite->Update();
	restartSprite->Update();
	titleSprite->Update();
}

void Menu::Draw(ID3D12GraphicsCommandList* cmdList)
{
	if (isMenu) {
		baseSprite->Draw(cmdList);
		restartSprite->Draw(cmdList);
		titleSprite->Draw(cmdList);
	}
}

void Menu::SetPosition(XMFLOAT2 position)
{
	//�x�[�X���ړ�
	baseSprite->SetPosition(position);
	//�X�v���C�g�̈ړ���
	XMFLOAT2 positionMove = XMFLOAT2{ this->position.x - position.x , this->position.y - position.y };

	//�ړ����������Ǐ]���Ĉړ�������
	XMFLOAT2 restartPosition = XMFLOAT2{ restartSprite->GetPosition().x - positionMove.x ,  restartSprite->GetPosition().y - positionMove.y };
	restartSprite->SetPosition(restartPosition);

	XMFLOAT2 titlePosition = XMFLOAT2{ titleSprite->GetPosition().x - positionMove.x ,  titleSprite->GetPosition().y - positionMove.y };
	titleSprite->SetPosition(titlePosition);
}
