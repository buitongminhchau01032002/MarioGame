#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_GRAVITY_SLEEP 0.002f
#define KOOPA_GRAVITY_FLY 0.0004f
#define KOOPA_WALKING_SPEED 0.03f
#define KOOPA_SLEEP_SPEED 0.2f
#define KOOPA_FLY_SPEED 0.19f
#define KOOPA_DEFLECT_SPEED 0.45f

#define KOOPA_TYPE_RED 1
#define KOOPA_TYPE_GREEN 2
#define KOOPA_TYPE_FLY 3

#define KOOPA_SLEEP_DURATION 10000

#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 27
#define KOOPA_BBOX_HEIGHT_SLEEP 8
#define KOOPA_BBOX_WIDTH_SLEEP 12

#define KOOPA_STATE_FLY	500
#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_SLEEPING 200
#define KOOPA_STATE_SLEEP 300
#define KOOPA_STATE_CARRIED 400

#define ID_ANI_KOOPA_RED_WALKING_LEFT 30001
#define ID_ANI_KOOPA_RED_WALKING_RIGHT 30000
#define ID_ANI_KOOPA_RED_SLEEPING 30002
#define ID_ANI_KOOPA_RED_SLEEP 30003

#define ID_ANI_KOOPA_GREEN_WALKING_LEFT 31001
#define ID_ANI_KOOPA_GREEN_WALKING_RIGHT 31000
#define ID_ANI_KOOPA_GREEN_SLEEPING 31002
#define ID_ANI_KOOPA_GREEN_SLEEP 31003

#define ID_ANI_KOOPA_FLY_WALKING_LEFT 32001
#define ID_ANI_KOOPA_FLY_WALKING_RIGHT 32000

class CKoopa :
    public CGameObject
{
protected:
	float ay;
	ULONGLONG sleepStart = 0;
	int type;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return state != KOOPA_STATE_CARRIED; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBox(LPCOLLISIONEVENT e);
	void OnCollisionWithLightBrick(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y, int type);
	void MoveInSleep(int direction);
	void SetState(int state);
};

