#include "LightBrick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Animations.h"
#include "debug.h"

CLightBrickDebris::CLightBrickDebris(int x, int y, float vx, float vy) {
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	removeStart = GetTickCount64();
}

void CLightBrickDebris::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	vy += LIGHT_BRICK_DEBRIS_GAVITY * dt;
	x += vx * dt;
	y += vy * dt;
	//DebugOutTitle(L"x: %f, y: %f", x, y);
	if (GetTickCount64() - removeStart > LIGHT_BRICK_DEBRIS_DURATION)
	{
		this->Delete();
	}
}

void CLightBrickDebris::Render() {
	LPANIMATION ani = CAnimations::GetInstance()->Get(200001);
	ani->Render(x, y);
}

CLightBrick::CLightBrick(int xCell, int yCell, int group, bool sm) {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	this->xCell = xCell;
	this->yCell = yCell;
	x = float(xCell * blockSize + blockSize / 2);
	y = float(yCell * blockSize + blockSize / 2 - 1);
	this->group = group;
	this->sm = sm;
}

void CLightBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	int ajust = 0;
	if (sm) {
		ajust = 1;
	}
	l = float(xCell * blockSize);
	t = float(yCell * blockSize);
	r = float(xCell * blockSize + blockSize - ajust);
	b = float(yCell * blockSize + blockSize);
}

void CLightBrick::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_LIGHT_BRICK);
	ani->Render(x, y);
	//RenderBoundingBox();
}

void CLightBrick::Break() {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT>& objects = s->GetObjects();
	objects.push_back(new CLightBrickDebris(x, y, LIGHT_BRICK_DEBRIS_SPEED_X, 0.0f));
	objects.push_back(new CLightBrickDebris(x, y, -LIGHT_BRICK_DEBRIS_SPEED_X, 0.0f));
	objects.push_back(new CLightBrickDebris(x, y, LIGHT_BRICK_DEBRIS_SPEED_X, -LIGHT_BRICK_DEBRIS_SPEED_Y));
	objects.push_back(new CLightBrickDebris(x, y, -LIGHT_BRICK_DEBRIS_SPEED_X, -LIGHT_BRICK_DEBRIS_SPEED_Y));
	this->Delete();
}