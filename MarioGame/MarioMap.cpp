#include "MarioMap.h"
#include "Animations.h"
#include "Map.h"
#include "debug.h"

CMarioMap::CMarioMap(int xCell, int yCell) 
	{
		float x, y;
		this->xCell = xCell;
		this->yCell = yCell;
		GetObjectXY(xCell, yCell, x, y);
		SetPosition(x, y);
	}


void CMarioMap::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(1200);
	ani->Render(x, y);
}

void CMarioMap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (state != MARIO_MAP_STATE_MOVING) {
		return;
	}

	int isDone = 0;
	float vx, vy;
	float xTo, yTo;
	int dy = abs(yCellTo - yCell);
	int dx = abs(xCellTo - xCell);
	float d = sqrt(dx * dx + dy * dy);
	float k = v / d;
	GetObjectXY(xCellTo, yCellTo, xTo, yTo);

	if (xCellTo > xCell) {
		if (dy == 0) {
			vx = v;
		}
		else {
			vx = dx * k;
		}
		x += vx * dt;
		if (x >= xTo) {
			x = xTo;
			isDone++;
			xCell = xCellTo;
		}
	}
	else if (xCellTo < xCell) {
		if (dy == 0) {
			vx = -v;
		}
		else {
			vx = -dx * k;
		}
		x += vx * dt;
		if (x <= xTo) {
			x = xTo;
			isDone++;
			xCell = xCellTo;
		}
	}
	else {
		isDone++;
	}

	if (yCellTo > yCell) {
		if (dx == 0) {
			vy = v;
		}
		else {
			vy = dy * k;
		}
		y += vy * dt;
		if (y >= yTo) {
			y = yTo;
			isDone++;
			yCell = yCellTo;
		}
	}
	else if (yCellTo < yCell) {
		if (dx == 0) {
			vy = -v;
		}
		else {
			vy = -dy * k;
		}
		y += vy * dt;
		if (y <= yTo) {
			y = yTo;
			isDone++;
			yCell = yCellTo;
		}
	}
	else {
		isDone++;
	}

	DebugOutTitle(L"state: %d, isDone: %d", state, isDone);

	if (isDone == 2) {
		state = MARIO_MAP_STATE_NONE;
	}
}
