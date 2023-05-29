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
private: // エイリアス
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
	//キーボード
	Input* input;

	XMFLOAT2 position = {};

	//ベースの画像
	Sprite* baseSprite;
	//"チェックポイントに戻る"
	Sprite* restartSprite;
	//"タイトルに戻る"
	Sprite* titleSprite;
	//"メニューを閉じる"
	Sprite* closeSprite;

	//何を選択しているか
	int serect;

	//シーン切り替えをしたか
	bool isSerect;

	//音
	//選択音
	AudioManager* selectSE;
	float selectSEVolume = 0.1f;

	//決定音
	AudioManager* pickSE;
	float pickSEVolume = 0.05f;
};