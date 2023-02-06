#pragma once

#define NUMBER_FONT_WIDTH 8.0f

#define ID_SPRITE_NUMBER_FONT_0 -10200
#define ID_SPRITE_NUMBER_FONT_1 -10201
#define ID_SPRITE_NUMBER_FONT_2 -10202
#define ID_SPRITE_NUMBER_FONT_3 -10203
#define ID_SPRITE_NUMBER_FONT_4 -10204
#define ID_SPRITE_NUMBER_FONT_5 -10205
#define ID_SPRITE_NUMBER_FONT_6 -10206
#define ID_SPRITE_NUMBER_FONT_7 -10207
#define ID_SPRITE_NUMBER_FONT_8 -10208
#define ID_SPRITE_NUMBER_FONT_9 -10209

class CNumberFont {
private:
	int align;
	int size;
	float x;
	float y;
public:
	CNumberFont(float x, float y, int align = 1,int size = -1); //1: left, -1: right
	void Render(int number);
};


