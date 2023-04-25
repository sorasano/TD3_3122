#pragma once
#include "Windows.h"
#include "cstdint"

static const int window_width = 1280;	//����
static const int window_height = 720;	//����

class WinApp
{
public:
	//�E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	static WinApp* GetInstance();
	//�E�B���h�E����
	void CreateWindow_(const wchar_t* title);
	//�E�B���h�E�N���X��o�^����
	void deleteWindow();
	//�Q�b�^�[
	HWND GetHwnd() { return hwnd; };
public:
	//�����o�ϐ�
	WNDCLASSEX w{};
	HWND hwnd;
};
