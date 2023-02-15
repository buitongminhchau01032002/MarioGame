#pragma once
#include "Coin.h"

#define COIN_FLY_UP_SPEED 0.4f
#define COIN_FLY_UP_GRAVITY 0.001f
#define COIN_FLY_UP_DURATION 600

class CCoinFlyUp :
    public CCoin
{
private:
    ULONGLONG start;
public:
    CCoinFlyUp(float x, float y) : CCoin(x, y) {
        start = GetTickCount64();
        vy = -COIN_FLY_UP_SPEED;
    }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

