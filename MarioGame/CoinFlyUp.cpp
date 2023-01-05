#include "CoinFlyUp.h"

void CCoinFlyUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - start < COIN_FLY_UP_DURATION) {
		y -= dt * COIN_FLY_UP_SPEED;
	}
	else {
		this->Delete();
	}
}
