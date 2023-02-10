#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.01f
#define KOOPA_WALKING_SPEED 0.03f
#define KOOPA_SLEEP_SPEED 0.25f


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 27
#define KOOPA_BBOX_HEIGHT_SLEEP 14

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_SLEEPING 200
#define KOOPA_STATE_SLEEP 300

#define ID_ANI_KOOPA_WALKING_LEFT 30001
#define ID_ANI_KOOPA_WALKING_RIGHT 30000
#define ID_ANI_KOOPA_SLEEPING 30002
#define ID_ANI_KOOPA_SLEEP 30003

class CKoopa :
    public CGameObject
{
protected:
	float ay;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBox(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	void MoveInSleep(int direction);
};

