#pragma once
#include "DirectXMath.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include"CubeModel.h"
#include"CubeObject3D.h"
#include "Camera.h"
#include "AudioManager.h"

enum PlayerAction {
	WAIT,
	WALK,
	JUMP,
	CLIMB,
};

class Player
{

public:

	void Initialize(CubeObject3D* cubeObject);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);
	void DrawLightView(ID3D12GraphicsCommandList* cmdList);

	//リセット
	void Reset(XMFLOAT2 savePos);

	//セッター
	void SetPosition(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	void Setrotate(XMFLOAT3 rotate);
	void SetSRV(ID3D12DescriptorHeap* SRV);

	void SetAlpha(float alpha) { this->alpha = alpha; }
	void SetisDeath(int isDeath) { this->isDeath = isDeath; }
	void SetJump(bool isJump) { this->isJump = isJump; }
	void SetSwamp(bool inSwamp) { this->inSwamp = inSwamp; }
	void SetGravity(float gravity) { this->gravity = gravity; }

	static void SetInput(Input* input) { Player::input = input; }
	static void SetDXInput(DXInput* dxInput) { Player::dxInput = dxInput; }

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	//XMFLOAT3 GetScale() { return scale; }
	XMFLOAT3 GetRotate() { return rotate; }
	float GetSwampSpeed() { return swampSpeed; }
	float GetSpeed() { return speed; }
	float GetGravity() { return gravity; }
	bool GetinSwamp() { return inSwamp; }

	CubeObject3D *GetCubeObject() {return cubeObject_; }
	bool GetDeath() { return isDeath; }

	//死亡処理
	void Death();
	//ジャンプ処理
	void Jump();

	void SetisClear(bool isClear) { this->isClear = isClear; };
	//沼に入っているか
	void Swamp();
	//梯子に登っているか
	void Ladder();
	//押し戻し処理
	void pushback(CubeObject3D* cubeObject);
	//上の判定
	bool OntheBlock(CubeObject3D* cubeObject);
	//横の判定
	bool pushBlock(CubeObject3D* cubeObject);
	
private:
	
	XMFLOAT3 position = { 0.0f,2.0f,-1.0f};
	XMFLOAT3 scale = { 0.002f,0.001f,0.002f };
	XMFLOAT3 rotate = { 0.0f,0.0f,0.0f };

private:

	//入力
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;

	bool isInput = false;

	//fbx
	FbxObject3D* playerObject = nullptr;

	FbxModel* playerWaitModel = nullptr;
	FbxModel* playerWalkModel = nullptr;
	FbxModel* playerJumpModel = nullptr;
	FbxModel* playerClimbModel = nullptr;

	//移動スピード
	float speed = 0.1;

	//生きてるか
	bool isDeath = false;
	//ゴールしてるか
	bool isClear = false;
	//演出のアルファ値
	float alpha = 0.0f;

	//移動制限　沼に入っているか
	bool inSwamp = false;
	float swampSpeed = 0.05;

	//梯子
	//乗っているか
	bool onLadder = false;
	//当たっているか
	bool colLadder = false;

	//ジャンプ
	bool isJump = false;

	//重力
	float gravity = 0.0f;
	//ジャンプでかかる重力(ジャンプの高さは jumpGravity / gravitySpeed / 10 例: 0.2 / 0.01 / 10 = 2)
	float jumpGravity = 0.2;
	//重力加速度
	float gravitySpeed = 0.01f;

	//判定
	CubeObject3D* cubeObject_ = nullptr;
	XMFLOAT3 colposition = { 0,0,0 };
	XMFLOAT3 newposition = { 0,0,0 };


	//行動中の動き
	int action;
	//前フレームの動き
	int oldAction;

	//アニメーションするか
	bool stopAnimation = false;
	bool oldStopAnimation = false;

	//歩き
	AudioManager* walkingSE;
	float walkingSEVolume = 0.3f;
	//沼
	AudioManager* swampSE;
	float swampSEVolume = 0.5f;
	//ハシゴ
	AudioManager* climbSE;
	float climbSEVolume = 0.1f;
	//ジャンプ
	AudioManager* jumpSE;
	float jumpSEVolume = 0.3f;
	//着地
	AudioManager* landSE;
	float landSEVolume = 0.3f;
	bool air = false;
};

