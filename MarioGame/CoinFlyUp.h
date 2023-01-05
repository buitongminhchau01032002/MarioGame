#pragma once
#include "Coin.h"

#define COIN_FLY_UP_SPEED 0.2f
#define COIN_FLY_UP_DURATION 400

class CCoinFlyUp :
    public CCoin
{
private:
    ULONGLONG start;
public:
    CCoinFlyUp(float x, float y) : CCoin(x, y) {
        start = GetTickCount64();
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

