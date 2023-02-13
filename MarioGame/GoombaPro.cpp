#include "GoombaPro.h"

CGoombaPro::CGoombaPro(float x, float y) :CGoomba(x, y)
{
	lastFlying = GetTickCount64();
	level = GOOMBA_PRO_LEVEL_PRO;
}

void CGoombaPro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CGoomba::GetBoundingBox(left, top, right, bottom);
}

void CGoombaPro::Render()
{
	int aniId = ID_ANI_GOOMBA_PRO_WALKING;
	if (level == GOOMBA_PRO_LEVEL_PRO) {
		aniId = ID_ANI_GOOMBA_PRO_WALKING_PRO;
	}
	if (state == GOOMBA_STATE_FLYING) {
		aniId = ID_ANI_GOOMBA_PRO_FLYING;
	}
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_PRO_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoombaPro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGoomba::Update(dt, coObjects);
	if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_STRONG) {
		if (GetTickCount64() - lastFlying > GOOMBA_PRO_TIME_TO_FLY && level == GOOMBA_PRO_LEVEL_PRO) {
			lastFlying = GetTickCount64();
			vy = -GOOMBA_PRO_FLY_SPEED;
			state = GOOMBA_STATE_FLYING;
		}
	}
}

void CGoombaPro::OnCollisionWith(LPCOLLISIONEVENT e)
{
	CGoomba::OnCollisionWith(e);
	if (e->ny == -1)
	{
		vy = 0;
		state = GOOMBA_STATE_WALKING;
	}
}