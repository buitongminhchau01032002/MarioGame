#pragma once
#include "GameObject.h"

#define ID_ANI_MUSHROOM 70000
#define MUSHROOM_WIDTH 16
#define MUSHROOM_HEIGHT 16
#define MUSHROOM_STAY_DELAYING -1
#define MUSHROOM_STATE_STARTUP 0
#define MUSHROOM_STATE_MOVE 1
#define MUSHROOM_SPEED 0.03f
#define MUSHROOM_STARTUP_DELAY 500
#define MUSHROOM_STARTUP_SPEED 0.05f
#define MUSHROOM_GRAVITY 0.01f

class CMushroom :
    public CGameObject
{
private:
    ULONGLONG startDelay;
    float originY;
public:
    CMushroom(float x, float y):CGameObject(x, y) {
        state = MUSHROOM_STAY_DELAYING;
        startDelay = GetTickCount64();
        originY = y;
    }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void OnNoCollision(DWORD dt);
    int IsBlocking(){ return 0; }
    int IsCollidable()
    {
        return (state == MUSHROOM_STATE_MOVE);
    }
};

