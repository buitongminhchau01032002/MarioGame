#pragma once
#include "GameObject.h"

#define ID_ANI_LIGHT_BRICK 200000

class CLightBrick: public CGameObject {
protected:
	int xCell, yCell;
	int group;
public:
	CLightBrick(int xCell, int yCell, int group);
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return true; }
	void Break();
};

