#include "BottomBar.h"
#include "Sprites.h"
#include "Sprite.h"
#include "Game.h"

CBottomBar::CBottomBar()
{
	CGame* g = CGame::GetInstance();

	x = g->GetBackBufferWidth() / 2;
	y = g->GetBackBufferHeight() - BOTTOM_BAR_HEIGHT / 2;
}

void CBottomBar::Render() {
	
	LPSPRITE sprite = CSprites::GetInstance()->Get(ID_SPRITE_BOTTOM_BAR_BG);
	sprite->Draw(x, y, true);
}