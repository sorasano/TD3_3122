#include "Player.h"
#include "Sprite.h"
#pragma once
class Goal
{
public:
	//初期化
	void Initialize(Sprite* whiteSprite,Sprite* clearSprite, Player* player);
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//ゲッター
	bool GetIsClear() { return isClear; }
	//セッター
	void SetClearPos(float clearPos) {this->clearPos=clearPos; };

	static void SetInput(Input* input) { Goal::input = input; }
	static void SetDXInput(DXInput* dxInput) { Goal::dxInput = dxInput; }
private:
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	
	bool isClear = false;
	float clearPos = 10;
	float alpha = 0.0f;



	Player* player = nullptr;
	Sprite* whiteSprite = nullptr;
	Sprite* clearSprite = nullptr;
};

