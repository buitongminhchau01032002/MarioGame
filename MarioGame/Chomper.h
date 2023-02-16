#pragma once
#include "GameObject.h"

#define CHOMPER_BBOX_WIDTH 16
#define CHOMPER_BBOX_HEIGHT 32
#define CHOMPER_BBOX_HEIGHT_GREEN 24
#define CHOMPER_PUSH_HEIGHT 48
#define CHOMPER_PUSH_HEIGHT_GREEN 44
#define CHOMPER_GUN_Y_OFFSET -8
#define CHOMPER_GUN_Y_OFFSET_GREEN -4

#define CHOMPER_SHOW_SPEED 0.03f
#define CHOMPER_HIDDEN_TIME 3000
#define CHOMPER_SHOOTING_DURATION 800
#define CHOMPER_SHORT_DISTANCE	100

#define CHOMPER_DIE_DURATION 400

#define CHOMPER_STATE_HIDDEN 0
#define CHOMPER_STATE_SHOW 1
#define CHOMPER_STATE_SHOOTING 2
#define CHOMPER_STATE_DIE 3

#define CHOMPER_TYPE_RED 1
#define CHOMPER_TYPE_GREEN 2

#define ID_ANI_CHOMPER_LEFT_BOTTOM 80000
#define ID_ANI_CHOMPER_LEFT_BOTTOM_SHOOTING 80001
#define ID_ANI_CHOMPER_LEFT_TOP 81000
#define ID_ANI_CHOMPER_LEFT_TOP_SHOOTING 81001
#define ID_ANI_CHOMPER_RIGHT_TOP 82000
#define ID_ANI_CHOMPER_RIGHT_TOP_SHOOTING 82001
#define ID_ANI_CHOMPER_RIGHT_BOTTOM 83000
#define ID_ANI_CHOMPER_RIGHT_BOTTOM_SHOOTING 83001

#define ID_ANI_CHOMPER_GREEN_LEFT_BOTTOM 80010
#define ID_ANI_CHOMPER_GREEN_LEFT_BOTTOM_SHOOTING 80011
#define ID_ANI_CHOMPER_GREEN_LEFT_TOP 81010
#define ID_ANI_CHOMPER_GREEN_LEFT_TOP_SHOOTING 81011
#define ID_ANI_CHOMPER_GREEN_RIGHT_TOP 82010
#define ID_ANI_CHOMPER_GREEN_RIGHT_TOP_SHOOTING 82011
#define ID_ANI_CHOMPER_GREEN_RIGHT_BOTTOM 83010
#define ID_ANI_CHOMPER_GREEN_RIGHT_BOTTOM_SHOOTING 83011

#define ID_ANI_CHOMPER_PIPE 84000
#define ID_ANI_CHOMPER_DIE 89000

class CChomper :
    public CGameObject
{
	int type;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	float originY;
	float GetGunY();
	ULONGLONG pushTimerStart;
	ULONGLONG shootingTimerStart;
	ULONGLONG dieStart;

public:
	CChomper(float x, float y, int type);
	void GetShowBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};

