#pragma once

#include "GameObject.h"

#define BOTTOM_BAR_HEIGHT 48
#define ID_SPRITE_BOTTOM_BAR_BG -10000

class CBottomBar : public CGameObject{
public:
	CBottomBar();
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	};
	int IsBlocking() { return 0; }
};

