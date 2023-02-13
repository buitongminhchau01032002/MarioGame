#pragma once
#include "GameObject.h"

#define CHOMPER_SMALL_BBOX_WIDTH 16
#define CHOMPER_SMALL_BBOX_HEIGHT 24
#define CHOMPER_SMALL_PUSH_HEIGHT 28

#define CHOMPER_SMALL_SHOW_SPEED 0.03f
#define CHOMPER_SMALL_HIDDEN_TIME 2500
#define CHOMPER_SMALL_SHOW_TIME 2000

#define CHOMPER_SMALL_STATE_HIDDEN 0
#define CHOMPER_SMALL_STATE_SHOW 1
#define CHOMPER_SMALL_STATE_DIE 3

#define ID_ANI_CHOMPER_SMALL 87000

#define ID_ANI_CHOMPER_SMALL_PIPE 87009

class CChomperSmall :
	public CGameObject
{
	int type;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	float originY;
	ULONGLONG pushTimerStart;
	ULONGLONG pullTimerStart;

public:
	CChomperSmall(float x, float y);
	void GetShowBox(float& left, float& top, float& right, float& bottom);
};

