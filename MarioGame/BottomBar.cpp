#include "BottomBar.h"
#include "Sprites.h"
#include "Sprite.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Animation.h"

CBottomBar::CBottomBar()
{
	CGame* g = CGame::GetInstance();

	x = g->GetBackBufferWidth() / 2;
	y = g->GetBackBufferHeight() - BOTTOM_BAR_HEIGHT / 2;
	timeText = new CNumberFont(x + BOTTOM_BAR_TIME_X, y + BOTTOM_BAR_TIME_Y, BOTTOM_BAR_TIME_ALIGN, BOTTOM_BAR_TIME_SIZE);
	coinText = new CNumberFont(x + BOTTOM_BAR_COIN_X, y + BOTTOM_BAR_COIN_Y, BOTTOM_BAR_COIN_ALIGN, BOTTOM_BAR_COIN_SIZE);
	coinValueText = new CNumberFont(x + BOTTOM_BAR_COIN_VALUE_X, y + BOTTOM_BAR_COIN_VALUE_Y, BOTTOM_BAR_COIN_VALUE_ALIGN, BOTTOM_BAR_COIN_VALUE_SIZE);
	heartText = new CNumberFont(x + BOTTOM_BAR_HEART_X, y + BOTTOM_BAR_HEART_Y, BOTTOM_BAR_HEART_ALIGN, BOTTOM_BAR_HEART_SIZE);
	worldText = new CNumberFont(x + BOTTOM_BAR_WORLD_X, y + BOTTOM_BAR_WORLD_Y, BOTTOM_BAR_WORLD_ALIGN, BOTTOM_BAR_WORLD_SIZE);
}

void CBottomBar::Render() {
	CGame* g = CGame::GetInstance();
	CSprites* sprites = CSprites::GetInstance();

	// bg
	LPSPRITE spriteBg = sprites->Get(ID_SPRITE_BOTTOM_BAR_BG);
	spriteBg->Draw(x, y, true);

	// power
	int powerProgress = 0;
	LPPLAYSCENE scene = dynamic_cast<LPPLAYSCENE>(g->GetCurrentScene());
	if (scene) {
		CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
		if (mario) {
			powerProgress = mario->GetPowerProgress();
		}
	}

	for (int i = 1; i <= 6; i++) {
		LPSPRITE spriteArrow;
		if (powerProgress >= i) {
			spriteArrow = sprites->Get(ID_SPRITE_BOTTOM_BAR_POWER_ARROW_WHITE);
		}
		else {
			spriteArrow = sprites->Get(ID_SPRITE_BOTTOM_BAR_POWER_ARROW_BLACK);
		}
		spriteArrow->Draw(x + BOTTOM_BAR_POWER_ARROW_WIDTH * (i - 1) + BOTTOM_BAR_POWER_X, y + BOTTOM_BAR_POWER_Y, true);
	}
	if (powerProgress != 7) {
		LPSPRITE spritePowerP = sprites->Get(ID_SPRITE_BOTTOM_BAR_POWER_P_BLACK);
		spritePowerP->Draw(x + BOTTOM_BAR_POWER_ARROW_WIDTH * 5.5 + BOTTOM_BAR_POWER_P_WIDTH/2 + BOTTOM_BAR_POWER_X, y + BOTTOM_BAR_POWER_Y, true);
	}
	else {
		LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_BOTTOM_BAR_POWER_P);
		ani->Render(x + BOTTOM_BAR_POWER_ARROW_WIDTH * 5.5 + BOTTOM_BAR_POWER_P_WIDTH / 2 + BOTTOM_BAR_POWER_X, y + BOTTOM_BAR_POWER_Y, true);
	}

	// time
	int time = 0;
	if (scene) {
		CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
		if (mario) {
			time = mario->GetTime();
		}
	}
	timeText->Render(time);

	coinText->Render(g->GetCoin());
	coinValueText->Render(g->GetCoinValue());

	heartText->Render(g->GetHeart());
	worldText->Render(1);

	DebugOutTitle(L"power: %d", powerProgress);
}