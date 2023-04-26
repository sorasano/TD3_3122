#pragma once
#include "Player.h"

class Autosave
{

public:

	void Initialize(Player* player);

	void Update();

	XMFLOAT2 GetSavePos() { return savePos[nowSavePos]; }

private:
	
	//�Z�[�u�n�_��
	static const int saveSize = 22;

	XMFLOAT2 savePos[saveSize];

	//���ǂ̃Z�[�u�n�_��
	int nowSavePos = 0;

	Player *player;

};

