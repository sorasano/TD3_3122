#pragma once

#include"DirectXCommon.h"
#include"windows.h"
#include"WinApp.h"

#define DIRECTINPUT_VERSION 0x0800		//DirectInput�̃o�[�W�����w��
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<wrl.h>
#include<Xinput.h>
#pragma comment(lib,"xinput.lib")

//////////////////////////////////////////
/////////////PAD�̃{�^���ꗗ////////////////
/////////////////////////////////////////
//XINPUT_GAMEPAD_DPAD_UP				�X�e�B�b�N��
//XINPUT_GAMEPAD_DPAD_DOWN				�X�e�B�b�N��
//XINPUT_GAMEPAD_DPAD_LEFT				�X�e�B�b�N��
//XINPUT_GAMEPAD_DPAD_RIGHT				�X�e�B�b�N�E
//XINPUT_GAMEPAD_START					�X�^�[�g�{�^��
//XINPUT_GAMEPAD_BACK					�Z���N�g�{�^��
//XINPUT_GAMEPAD_LEFT_THUMB				���X�e�B�b�N��������
//XINPUT_GAMEPAD_RIGHT_THUMB			�E�X�e�B�b�N��������
//XINPUT_GAMEPAD_LEFT_SHOULDER			L�{�^��(LB�͕�)
//XINPUT_GAMEPAD_RIGHT_SHOULDER			R�{�^��(RB�͕�)
//XINPUT_GAMEPAD_A						A�{�^��
//XINPUT_GAMEPAD_B						B�{�^��
//XINPUT_GAMEPAD_X						X�{�^��
//XINPUT_GAMEPAD_Y						Y�{�^��


class Input
{
public:

private:
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256]{};
	BYTE oldkey[256]{};
	WinApp* winApp_;

	Input();
	~Input();
public:

	//�R�s�[�R���X�g���N�^����
	Input(const Input& obj) = delete;
	//������Z�q�𖳌�
	Input& operator=(const Input& obj) = delete;

	//�C���X�^���X�A�N�Z�X��p�֐�
	static Input* GetInstance();

	XINPUT_STATE padState;
	XINPUT_STATE oldPadState;

	//������
	void Initialize(WinApp* winApp);

	//�X�V
	void Update();

	//�L�[�{�[�h�������
	bool IsKeyTrigger(BYTE key_);
	bool IsKeyPress(BYTE key_);
	bool IsKeyRelease(BYTE key_);

	//�p�b�h�������(�{�^��)
	bool IsPadTrigger(WORD Button);
	bool IsPadPress(WORD Button);
	bool IsPadRelease(WORD Button);

	//�p�b�h�������(���E�X�e�B�b�N)
	bool IsDownLStickLeft(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsTriggerLStickLeft(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownLStickRight(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsTriggerLStickRight(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownLStickUp(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsTriggerLStickUp(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsDownLStickDown(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	bool IsTriggerLStickDown(int deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

	bool IsDownRStickLeft(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsTriggerRStickLeft(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsDownRStickRight(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsTriggerRStickRight(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsDownRStickUp(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsTriggerRStickUp(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsDownRStickDown(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	bool IsTriggerRStickDown(int deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
};

