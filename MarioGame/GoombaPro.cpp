#include "GoombaPro.h"

CGoombaPro::CGoombaPro(float x, float y) :CGoomba(x, y)
{
	lastFlying = GetTickCount64();
	level = GOOMBA_PRO_LEVEL_PRO;
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

	CAnimations::GetInstance()->Get(aniId)->Render(x, y-4);
	//RenderBoundingBox();
}

void CGoombaPro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoombaPro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGoomba::Update(dt, coObjects);
	if (state != GOOMBA_STATE_DIE) {
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