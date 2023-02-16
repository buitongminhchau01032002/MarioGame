#pragma once
#include "GameObject.h"

#define CHOMPER_BULLET_BBOX_WIDTH 6
#define CHOMPER_BULLET_BBOX_HEIGHT 6
#define CHOMPER_BULLET_SPEED 0.05f


#define ID_ANI_CHOMPER_BULLET 88000
#define CHOMPER_BULLET_DURATION 10000

class CChomperBullet :
    public CGameObject
{
	ULONGLONG duration;
public:
    CChomperBullet(float x, float y, float dx, float dy);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
};

