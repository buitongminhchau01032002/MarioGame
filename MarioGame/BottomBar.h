#pragma once

#include "GameObject.h"
#include "NumberFont.h"

#define BOTTOM_BAR_HEIGHT 48.0f
#define BOTTOM_BAR_POWER_X -58.0f
#define BOTTOM_BAR_POWER_Y -8.0f
#define BOTTOM_BAR_POWER_ARROW_WIDTH 8.0f
#define BOTTOM_BAR_POWER_P_WIDTH 16.0f

#define BOTTOM_BAR_TIME_X 12.0f
#define BOTTOM_BAR_TIME_Y 0.0f


#define ID_SPRITE_BOTTOM_BAR_BG -10000
#define ID_SPRITE_BOTTOM_BAR_POWER_ARROW_BLACK -10100
#define ID_SPRITE_BOTTOM_BAR_POWER_ARROW_WHITE -10101
#define ID_SPRITE_BOTTOM_BAR_POWER_P_BLACK -10110
#define ID_ANI_BOTTOM_BAR_POWER_P -10100


class CBottomBar : public CGameObject{
private:
	CNumberFont* timeText;
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

