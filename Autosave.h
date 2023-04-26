#pragma once
#include "Player.h"

class Autosave
{

public:

	void Initialize(Player* player);

	void Update();

	XMFLOAT2 GetSavePos() { return savePos[nowSavePos]; }

private:
	
	//セーブ地点個数
	static const int saveSize = 22;

	XMFLOAT2 savePos[saveSize];

	//今どのセーブ地点か
	int nowSavePos = 0;

	Player *player;

};

