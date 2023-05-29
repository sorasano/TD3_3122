#include "Input.h"
#include<wrl.h>
#include<cassert>
using namespace Microsoft::WRL;
#include"ImguiManager.h"

Input::Input()
{
}

Input::~Input()
{
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	HRESULT result_;
	winApp_ = winApp;

	//DirectInput�̃C���X�^���X����
	result_ = DirectInput8Create(
		winApp_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result_));
	//�L�[�{�[�h�f�o�C�X����
	result_ = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result_));
	//���̓f�[�^�`���̃Z�b�g
	result_ = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result_));
	//�r�����䃌�x���̃Z�b�g
	result_ = keyboard->SetCooperativeLevel(
		winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result_));
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�O�t���[���̃L�[����ۑ�
	for (int i = 0; i < 256; i++) {
		oldkey[i] = key[i];
	}
	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);

	//�p�b�h�̐ڑ��m�F
	oldPadState = padState;
	DWORD flag = XInputGetState(0, &padState);


}

bool Input::IsKeyTrigger(BYTE key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsKeyPress(BYTE key_)
{
	return key[key_];
}

bool Input::IsKeyRelease(BYTE key_)
{
	return (!key[key_] && oldkey[key_]);
}

bool Input::IsPadTrigger(WORD Button)
{
	return (padState.Gamepad.wButtons & Button) && ((oldPadState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsPadPress(WORD Button)
{
	return padState.Gamepad.wButtons & Button;
}

bool Input::IsPadRelease(WORD Button)
{
	return (oldPadState.Gamepad.wButtons & Button) && ((padState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsDownLStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbLX < -deadZone;
}

bool Input::IsTriggerLStickLeft(int deadZone)
{
	return (padState.Gamepad.sThumbLX < -deadZone) && (oldPadState.Gamepad.sThumbLX >= -deadZone);
}

bool Input::IsDownLStickRight(int deadZone)
{
	return padState.Gamepad.sThumbLX > deadZone;
}

bool Input::IsTriggerLStickRight(int deadZone)
{
	return (padState.Gamepad.sThumbLX > deadZone) && (oldPadState.Gamepad.sThumbLX <= deadZone);
}

bool Input::IsDownLStickUp(int deadZone)
{
	return padState.Gamepad.sThumbLY > deadZone;
}

bool Input::IsTriggerLStickUp(int deadZone)
{
	return (padState.Gamepad.sThumbLY > deadZone) && (oldPadState.Gamepad.sThumbLY <= deadZone);
}

bool Input::IsDownLStickDown(int deadZone)
{
	return padState.Gamepad.sThumbLY < -deadZone;
}

bool Input::IsTriggerLStickDown(int deadZone)
{
	return (padState.Gamepad.sThumbLY < -deadZone) && (oldPadState.Gamepad.sThumbLY >= -deadZone);
}

bool Input::IsDownRStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbRX < -deadZone;
}

bool Input::IsTriggerRStickLeft(int deadZone)
{
	return (padState.Gamepad.sThumbRX < -deadZone) && (oldPadState.Gamepad.sThumbRX >= -deadZone);
}

bool Input::IsDownRStickRight(int deadZone)
{
	return padState.Gamepad.sThumbRX > deadZone;
}

bool Input::IsTriggerRStickRight(int deadZone)
{
	return (padState.Gamepad.sThumbRX > deadZone) && (oldPadState.Gamepad.sThumbRX <= deadZone);
}

bool Input::IsDownRStickUp(int deadZone)
{
	return padState.Gamepad.sThumbRY > deadZone;
}

bool Input::IsTriggerRStickUp(int deadZone)
{
	return (padState.Gamepad.sThumbRY > deadZone) && (oldPadState.Gamepad.sThumbRY <= deadZone);
}

bool Input::IsDownRStickDown(int deadZone)
{
	return padState.Gamepad.sThumbRY < -deadZone;
}

bool Input::IsTriggerRStickDown(int deadZone)
{
	return (padState.Gamepad.sThumbRY < -deadZone) && (oldPadState.Gamepad.sThumbRY >= -deadZone);
}
