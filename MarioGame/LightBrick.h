#pragma once
#include "GameObject.h"

#define LIGHT_BRICK_DEBRIS_GAVITY 0.0005f
#define LIGHT_BRICK_DEBRIS_SPEED_X 0.04f
#define LIGHT_BRICK_DEBRIS_SPEED_Y 0.2f
#define LIGHT_BRICK_DEBRIS_DURATION 3000

#define ID_ANI_LIGHT_BRICK 200000
#define ID_ANI_LIGHT_BRICK_DEBRIS 200001


class CLightBrickDebris : public CGameObject {
	ULONGLONG removeStart;
public:
	CLightBrickDebris(int xCell, int yCell, float vx, float vy);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	}
	int IsCollidable() { return false; }
	int IsBlocking() { return false; }
};

class CLightBrick: public CGameObject {
protected:
	int xCell, yCell;
	int group;
public:
	CLightBrick(int xCell, int yCell, int group);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return true; }
	void Break();
};

