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
	void Reset();

	//�Q�b�^�[
	bool GetIsClear() { return isClear; }
	bool GetIsEnd() { return isEnd; }

	//�Z�b�^�[
	void SetClearPos(float clearPos) {this->clearPos=clearPos; };

private:

	bool isClear = false;
	float clearPos = 10;
	float alpha = 0.0f;
	//���o���I�������
	bool isEnd = false;

	Player* player = nullptr;
	Sprite* whiteSprite = nullptr;
	Sprite* clearSprite = nullptr;
};

