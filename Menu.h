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
private: // エイリアス
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	void Initialize();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetPosition(XMFLOAT2 position);
	static void SetInput(Input* input) { Menu::input = input; }
	static void SetDXInput(DXInput* dxInput) { Menu::dxInput = dxInput; }
	int GetSerect() { return serect; }
	int GetIsSerect() { return isSerect; }
	void Reset();

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
	//"メニューを閉じる"
	Sprite* closeSprite;

	//何を選択しているか
	int serect;

	//シーン切り替えをしたか
	bool isSerect;

};