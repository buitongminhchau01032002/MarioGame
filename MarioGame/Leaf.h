#pragma once
#include "GameObject.h"

#define ID_ANI_LEAF 71000
#define LEAF_WIDTH 16
#define LEAF_HEIGHT 16
#define LEAF_GRAVITY 0.001f
#define LEAF_SPEED_Y 0.3f
#define LEAF_SPEED_FALL_MAX 0.01f
#define LEAF_SPEED_X 0.18f
#define LEAF_LIMIT_X 30

#define LEAF_DURATION 10000

class CLeaf :
    public CGameObject
{
    float originX;
    ULONGLONG duration;
public:
    CLeaf(float x, float y) :CGameObject(x, y) {   
        originX = x;
        vx = LEAF_SPEED_X;
        vy = -LEAF_SPEED_Y;
        duration = GetTickCount64();
    }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    int IsBlocking() { return 0; }
    int IsCollidable()
    {
        return 1;
    }
};

