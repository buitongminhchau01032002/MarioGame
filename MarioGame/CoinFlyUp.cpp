#include "CoinFlyUp.h"
#include "Game.h"
#include "PlayScene.h"
#include "Label.h"

void CCoinFlyUp::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - start < COIN_FLY_UP_DURATION) {
		vy += dt * COIN_FLY_UP_GRAVITY;
		y += dt * vy;
	}
	else {
		this->Delete();
		CGame* g = CGame::GetInstance();
		g->IncreaseCoinValue(100);
		g->IncreaseCoin(1);
		LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT>& objects = s->GetObjects();
		objects.push_back(new CLabel(x, y, LABEL_100));
	}
}
