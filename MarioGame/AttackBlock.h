#pragma once
#include "GameObject.h"

#define ATTACK_BLOCK_WIDTH 12
#define ATTACK_BLOCK_HEIGHT 8
#define ATTACK_BLOCK_OFFSET_Y 6
#define	ATTACK_BLOCK_DURATION 300

class CAttackBlock :
    public CGameObject

{
private:
	ULONGLONG timer;
	void OnOverlapWithGoomba(LPGAMEOBJECT obj);
	void OnOverlapWithKoopa(LPGAMEOBJECT obj);
	void OnOverlapWithChoomper(LPGAMEOBJECT obj);
	bool IsOverLap(float l, float t, float r, float b);
public:
	CAttackBlock(float x, float y) :CGameObject(x, y) { timer = GetTickCount64(); };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
};

