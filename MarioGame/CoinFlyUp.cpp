#include "CoinFlyUp.h"
#include "Game.h"

void CCoinFlyUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - start < COIN_FLY_UP_DURATION) {
		y -= dt * COIN_FLY_UP_SPEED;
	}
	else {
		this->Delete();
		CGame* g = CGame::GetInstance();
		g->IncreaseCoinValue(100);
		g->IncreaseCoin(1);
	}
}
