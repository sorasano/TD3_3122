#pragma once
//#include "dxgidebug.h"
#include <d3d12.h>

class Message
{
public:
	static Message* GetInstance();
	//X�{�^���ŏI�����b�Z����������Q�[�����[�v�𔲂��� 
	bool Update();
	//�Ăяo�������̃G���[���o��
	bool CheckError();
public:
	MSG msg{};
};

