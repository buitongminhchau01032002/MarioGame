#include "Label.h"
#include "Sprites.h"

void CLabel::Render()
{
	CSprites::GetInstance()->Get(ID_SPRITE_LABEL + value)->Draw(x, y);
}
