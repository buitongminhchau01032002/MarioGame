#include "MushroomGreen.h"
#include "debug.h"
#include "PlayScene.h"

void CMushroomGreen::Render()
{
	if (state == MUSHROOM_GREEN_STAY_DELAYING) return;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MUSHROOM_GREEN)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroomGreen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;

	if (GetTickCount64() - startDelay < MUSHROOM_GREEN_STARTUP_DELAY) return;
	if (state == MUSHROOM_GREEN_STAY_DELAYING) state = MUSHROOM_GREEN_STATE_STARTUP;
	if (state == MUSHROOM_GREEN_STATE_STARTUP) {
		y -= dt * MUSHROOM_GREEN_STARTUP_SPEED;
		if (originY - y > MUSHROOM_GREEN_HEIGHT) {
			state = MUSHROOM_GREEN_STATE_MOVE;
		}
	}
	else { // Moving
		vx = MUSHROOM_GREEN_SPEED;
		vy = dt * MUSHROOM_GREEN_GRAVITY;
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CMushroomGreen::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_GREEN_WIDTH / 2;
	t = y - MUSHROOM_GREEN_HEIGHT / 2;
	r = l + MUSHROOM_GREEN_WIDTH;
	b = t + MUSHROOM_GREEN_HEIGHT - 1;
}

void CMushroomGreen::OnNoCollision(DWORD dt) {
	if (state != MUSHROOM_GREEN_STATE_MOVE) return;
	x += vx * dt;
	y += vy * dt;
}