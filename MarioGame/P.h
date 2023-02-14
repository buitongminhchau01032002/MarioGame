#pragma once
#include "GameObject.h"

#define ID_ANI_P_SHOW 72000
#define ID_ANI_P_DOWN 72001

#define P_WIDTH 16
#define P_HEIGHT 16

#define P_STATE_DELAYING -1
#define P_STATE_STARTUP 0
#define P_STATE_SHOW 1
#define P_STATE_DOWN 2

#define P_STARTUP_DELAY 500
#define P_STARTUP_SPEED 0.05f

class CP :
    public CGameObject
{
private:
    ULONGLONG startDelay;
    float originY;
public:
    CP(float x, float y) :CGameObject(x, y) {
        state = P_STATE_DELAYING;
        startDelay = GetTickCount64();
        originY = y;
    }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    int IsBlocking() { return state == P_STATE_SHOW; }
    int IsCollidable()
    {
        return (state == P_STATE_SHOW);
    }
    void Press();
};

