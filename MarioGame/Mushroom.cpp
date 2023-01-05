#include "Mushroom.h"
#include "debug.h"

void CMushroom::Render()
{
	if (state == MUSHROOM_STAY_DELAYING) return;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MUSHROOM)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - startDelay < MUSHROOM_STARTUP_DELAY) return;
	if (state == MUSHROOM_STAY_DELAYING) state = MUSHROOM_STATE_STARTUP;
	if (state == MUSHROOM_STATE_STARTUP) {
		y -= dt * MUSHROOM_STARTUP_SPEED;
		if (originY - y > MUSHROOM_HEIGHT) {
			state = MUSHROOM_STATE_MOVE;
		}
	}
	else { // Moving
		vx = -MUSHROOM_SPEED;
		vy = dt * MUSHROOM_GRAVITY;
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_WIDTH / 2;
	t = y - MUSHROOM_HEIGHT / 2;
	r = l + MUSHROOM_WIDTH;
	b = t + MUSHROOM_HEIGHT;
}

void CMushroom::OnNoCollision(DWORD dt) {
	if (state != MUSHROOM_STATE_MOVE) return;
	x += vx * dt;
	y += vy * dt;
}