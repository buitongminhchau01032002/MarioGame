#include "NumberFont.h"
#include "Sprites.h"

CNumberFont::CNumberFont(float x, float y, int align, int size)
{
	this->x = x;
	this->y = y;
	this->align = align;
	this->size = size;
}

int getSpriteIdNum(int num) {
	switch (num)
	{
	case 0:
		return ID_SPRITE_NUMBER_FONT_0;
	case 1:
		return ID_SPRITE_NUMBER_FONT_1;
	case 2:
		return ID_SPRITE_NUMBER_FONT_2;
	case 3:
		return ID_SPRITE_NUMBER_FONT_3;
	case 4:
		return ID_SPRITE_NUMBER_FONT_4;
	case 5:
		return ID_SPRITE_NUMBER_FONT_5;
	case 6:
		return ID_SPRITE_NUMBER_FONT_6;
	case 7:
		return ID_SPRITE_NUMBER_FONT_7;
	case 8:
		return ID_SPRITE_NUMBER_FONT_8;
	case 9:
		return ID_SPRITE_NUMBER_FONT_9;

	default:
		break;
	}
}

int countDigit(int n)
{
	if (n == 0)
		return 1;
	int count = 0;
	while (n != 0) {
		n = n / 10;
		++count;
	}
	return count;
}

void CNumberFont::Render(int number)
{
	CSprites* sprites = CSprites::GetInstance();
	int digit = countDigit(number);
	if (align == 1) {
		int index = digit-1;
		if (size > digit) {
			index = size - 1;
			for (int i = 0; i < size-digit; i++) {
				sprites->Get(ID_SPRITE_NUMBER_FONT_0)->Draw(x + NUMBER_FONT_WIDTH * i, y, true);
			}
		}
		int numberTemp = number;
		if (numberTemp == 0) {
			sprites->Get(getSpriteIdNum(0))->Draw(x + NUMBER_FONT_WIDTH * index, y, true);
		}
		while (numberTemp != 0) {
			int num = numberTemp % 10;

			sprites->Get(getSpriteIdNum(num))->Draw(x + NUMBER_FONT_WIDTH * index, y, true);

			numberTemp /= 10;
			index--;
		}
	}
	else {
		
		int numberTemp = number;
		if (numberTemp == 0) {
			sprites->Get(getSpriteIdNum(0))->Draw(x + NUMBER_FONT_WIDTH * 0, y, true);
		}
		int index = 0;
		while (numberTemp != 0) {
			int num = numberTemp % 10;

			sprites->Get(getSpriteIdNum(num))->Draw(x + NUMBER_FONT_WIDTH * (-index), y, true);

			numberTemp /= 10;
			index++;
		}
		if (size > digit) {
			for (int i = index; i < size - digit + index; i++) {
				sprites->Get(ID_SPRITE_NUMBER_FONT_0)->Draw(x + NUMBER_FONT_WIDTH * (- i), y, true);
			}
		}
	}
}
