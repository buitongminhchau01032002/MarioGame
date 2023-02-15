#include "Leaf.h"

void CLeaf::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_LEAF)->Render(x, y);
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - duration > LEAF_DURATION) this->Delete();

	vy += LEAF_GRAVITY * dt;
	if (vy > LEAF_SPEED_FALL_MAX) {
		vy = LEAF_SPEED_FALL_MAX;
	}
	if (x - originX > LEAF_LIMIT_X) {
		vx = -LEAF_SPEED_X;
	}
	if (originX - x > LEAF_LIMIT_X) {
		vx = LEAF_SPEED_X;
	}

	x += vx * dt;
	y += vy * dt;
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x -LEAF_WIDTH / 2;
	t = y -LEAF_HEIGHT / 2;
	r = l +LEAF_WIDTH;
	b = t +LEAF_HEIGHT;
}