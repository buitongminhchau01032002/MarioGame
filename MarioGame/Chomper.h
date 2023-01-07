#pragma once
#include "GameObject.h"

#define CHOMPER_BBOX_WIDTH 16
#define CHOMPER_BBOX_HEIGHT 32
#define CHOMPER_PUSH_HEIGHT 48
#define CHOMPER_GUN_X_OFFSET -8

#define CHOMPER_SHOW_SPEED 0.03f
#define CHOMPER_HIDDEN_TIME 3000
#define CHOMPER_SHOOTING_DURATION 800
#define CHOMPER_SHORT_DISTANCE	100

#define CHOMPER_STATE_HIDDEN 0
#define CHOMPER_STATE_SHOW 1
#define CHOMPER_STATE_SHOOTING 2

#define ID_ANI_CHOMPER_LEFT_BOTTOM 80000
#define ID_ANI_CHOMPER_LEFT_BOTTOM_SHOOTING 80001
#define ID_ANI_CHOMPER_LEFT_TOP 81000
#define ID_ANI_CHOMPER_LEFT_TOP_SHOOTING 81001
#define ID_ANI_CHOMPER_RIGHT_TOP 82000
#define ID_ANI_CHOMPER_RIGHT_TOP_SHOOTING 82001
#define ID_ANI_CHOMPER_RIGHT_BOTTOM 83000
#define ID_ANI_CHOMPER_RIGHT_BOTTOM_SHOOTING 83001

#define ID_ANI_CHOMPER_PIPE 84000

class CChomper :
    public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	float originY;
	float GetGunY();
	ULONGLONG pushTimerStart;
	ULONGLONG shootingTimerStart;

public:
	CChomper(float x, float y);
};

