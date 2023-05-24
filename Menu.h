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
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(XMFLOAT2 position);

private:
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;

	XMFLOAT2 position = {};

	//ベースの画像
	Sprite* baseSprite;
	//"チェックポイントに戻る"
	Sprite* restartSprite;
	//"タイトルに戻る"
	Sprite* titleSprite;

	//メニューを開いているか
	bool isMenu;

	//何を選択しているか
	int serect;

};

