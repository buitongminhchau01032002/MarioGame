#pragma once
#include "GameObject.h"

#define LABEL_DURATION 600
#define LABEL_SPEED 0.03f

#define LABEL_100 1
#define LABEL_200 2
#define LABEL_400 3
#define LABEL_800 4
#define LABEL_1000 5
#define LABEL_2000 6
#define LABEL_4000 7
#define LABEL_8000 8
#define LABEL_1UP 9

#define ID_SPRITE_LABEL 20

class CLabel :
    public CGameObject
{
private:
    ULONGLONG showStart;
    int value;
public:
    CLabel(float x, float y, int value) :CGameObject(x, y) {
        showStart = GetTickCount64();
        this->value = value;
    }
    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
        y -= LABEL_SPEED * dt;
        if (GetTickCount64() - showStart > LABEL_DURATION) this->Delete();
    }
    void GetBoundingBox(float& l, float& t, float& r, float& b) {
        l = 0;
        t = 0;
        r = 0;
        b = 0;
    };
    int IsCollidable()
    {
        return false;
    }
};

