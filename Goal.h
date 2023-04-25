#include "Player.h"
#include "Sprite.h"
#pragma once
class Goal
{
public:
	//������
	void Initialize(Sprite* whiteSprite,Sprite* clearSprite, Player* player);
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�Q�b�^�[
	bool GetIsClear() { return isClear; }
	//�Z�b�^�[
	void SetClearPos(float clearPos) {this->clearPos=clearPos; };
private:
	bool isClear = false;
	float clearPos = 10;
	float alpha = 0.0f;

	Player* player = nullptr;
	Sprite* whiteSprite = nullptr;
	Sprite* clearSprite = nullptr;
};

