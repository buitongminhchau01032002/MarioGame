#pragma once
#include "GameObject.h"

#define ATTACK_BLOCK_WIDTH 12
#define ATTACK_BLOCK_HEIGHT 8
#define ATTACK_BLOCK_OFFSET_Y 6
#define	ATTACK_BLOCK_DURATION 150

#define ID_SPRITE_ATTACK_EFFECT -999998
#define ATTACK_EFFECT_DURATION 80

class CAttackEffect : public CGameObject {
	ULONGLONG timer;
public:
	CAttackEffect(float x, float y) :CGameObject(x, y) {
		timer = GetTickCount64();
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
		if (GetTickCount64() - timer > ATTACK_EFFECT_DURATION) {
			this->Delete();
			return;
		}
	}
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; };
};

class CAttackBlock :
    public CGameObject

{
private:
	ULONGLONG timer;
	void OnOverlapWithGoomba(LPGAMEOBJECT obj);
	void OnOverlapWithKoopa(LPGAMEOBJECT obj);
	void OnOverlapWithChomper(LPGAMEOBJECT obj);
	void OnOverlapWithChomperSmall(LPGAMEOBJECT obj);
	void OnOverlapWithLightBrick(LPGAMEOBJECT obj);
	void OnOverlapWithQuestionBox(LPGAMEOBJECT obj);
	bool IsOverLap(float l, float t, float r, float b);
	void AddEffect();
	int nx;
public:
	CAttackBlock(float x, float y, int nx) :CGameObject(x, y) { timer = GetTickCount64(); this->nx = nx; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; };
	void AddLabel(int value);
};

