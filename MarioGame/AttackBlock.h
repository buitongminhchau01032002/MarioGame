#pragma once
#include "GameObject.h"

#define ATTACK_BLOCK_WIDTH 8
#define ATTACK_BLOCK_HEIGHT 24
#define	ATTACK_BLOCK_DURATION 100

class CAttackBlock :
    public CGameObject

{
private:
	ULONGLONG timer;
public:
	CAttackBlock(float x, float y) :CGameObject(x, y) { timer = GetTickCount64(); };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
};

