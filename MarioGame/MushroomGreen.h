#pragma once
#include "GameObject.h"

#define ID_ANI_MUSHROOM_GREEN 73000
#define MUSHROOM_GREEN_WIDTH 16
#define MUSHROOM_GREEN_HEIGHT 16
#define MUSHROOM_GREEN_STAY_DELAYING -1
#define MUSHROOM_GREEN_STATE_STARTUP 0
#define MUSHROOM_GREEN_STATE_MOVE 1
#define MUSHROOM_GREEN_SPEED 0.04f
#define MUSHROOM_GREEN_STARTUP_DELAY 500
#define MUSHROOM_GREEN_STARTUP_SPEED 0.05f
#define MUSHROOM_GREEN_GRAVITY 0.01f

class CMushroomGreen :
    public CGameObject
{
private:
    ULONGLONG startDelay;
    float originY;
public:
    CMushroomGreen(float x, float y):CGameObject(x, y) {
        state = MUSHROOM_GREEN_STAY_DELAYING;
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
        return (state == MUSHROOM_GREEN_STATE_MOVE);
    }
};

