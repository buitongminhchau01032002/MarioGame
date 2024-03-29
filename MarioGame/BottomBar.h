#pragma once

#include "GameObject.h"
#include "NumberFont.h"

#define BOTTOM_BAR_HEIGHT 48.0f
#define BOTTOM_BAR_POWER_X -61.0f
#define BOTTOM_BAR_POWER_Y -8.0f
#define BOTTOM_BAR_POWER_ARROW_WIDTH 8.0f
#define BOTTOM_BAR_POWER_P_WIDTH 16.0f

#define BOTTOM_BAR_TIME_X 12.0f
#define BOTTOM_BAR_TIME_Y 0.0f
#define BOTTOM_BAR_TIME_SIZE 3
#define BOTTOM_BAR_TIME_ALIGN 1

#define BOTTOM_BAR_COIN_X 28.0f
#define BOTTOM_BAR_COIN_Y -8.0f
#define BOTTOM_BAR_COIN_SIZE -1
#define BOTTOM_BAR_COIN_ALIGN -1

#define BOTTOM_BAR_COIN_VALUE_X -60.0f
#define BOTTOM_BAR_COIN_VALUE_Y 0.0f
#define BOTTOM_BAR_COIN_VALUE_SIZE 7
#define BOTTOM_BAR_COIN_VALUE_ALIGN 1

#define BOTTOM_BAR_HEART_X -76.0f
#define BOTTOM_BAR_HEART_Y -0.0f
#define BOTTOM_BAR_HEART_SIZE -1
#define BOTTOM_BAR_HEART_ALIGN 1

#define BOTTOM_BAR_WORLD_X -76.0f
#define BOTTOM_BAR_WORLD_Y -8.0f
#define BOTTOM_BAR_WORLD_SIZE -1
#define BOTTOM_BAR_WORLD_ALIGN -1

#define ID_SPRITE_BOTTOM_BAR_BG -10000
#define ID_SPRITE_BOTTOM_BAR_POWER_ARROW_BLACK -10100
#define ID_SPRITE_BOTTOM_BAR_POWER_ARROW_WHITE -10101
#define ID_SPRITE_BOTTOM_BAR_POWER_P_BLACK -10110
#define ID_ANI_BOTTOM_BAR_POWER_P -10100


class CBottomBar : public CGameObject{
private:
	CNumberFont* timeText;
	CNumberFont* coinText;
	CNumberFont* coinValueText;
	CNumberFont* heartText;
	CNumberFont* worldText;
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

