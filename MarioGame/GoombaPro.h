#pragma once
#include "Goomba.h"

#define ID_ANI_GOOMBA_PRO_WALKING_PRO 50003
#define ID_ANI_GOOMBA_PRO_WALKING 50000
#define ID_ANI_GOOMBA_PRO_DIE 50001
#define ID_ANI_GOOMBA_PRO_FLYING 50002

#define GOOMBA_PRO_LEVEL_PRO 1
#define GOOMBA_PRO_LEVEL_NORMAL 2

#define GOOMBA_STATE_FLYING 300

#define GOOMBA_PRO_TIME_TO_FLY 2100
#define GOOMBA_PRO_FLY_SPEED 0.4f

class CGoombaPro :
    public CGoomba
{
private:
    int level;
    ULONGLONG lastFlying;
public:
    CGoombaPro(float x, float y);
    void Render();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void OnCollisionWith(LPCOLLISIONEVENT e);
};

