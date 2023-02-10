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
}

void CLightBrickDebris::Render() {

}

CLightBrick::CLightBrick(int xCell, int yCell, int group) {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	this->xCell = xCell;
	this->yCell = yCell;
	x = float(xCell * blockSize + blockSize / 2);
	y = float(yCell * blockSize + blockSize / 2 - 1);
	this->group = group;
}

void CLightBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	l = float(xCell * blockSize);
	t = float(yCell * blockSize);
	r = float(xCell * blockSize + blockSize);
	b = float(yCell * blockSize + blockSize);
}

void CLightBrick::Render()
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_LIGHT_BRICK);
	ani->Render(x, y);
	RenderBoundingBox();
}

void CLightBrick::Break() {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT>& objects = s->GetObjects();
	objects.push_back(new CLightBrickDebris(x, y, LIGHT_BRICK_DEBRIS_SPEED_X, 0));
	objects.push_back(new CLightBrickDebris(x, y, -LIGHT_BRICK_DEBRIS_SPEED_X, 0));
	objects.push_back(new CLightBrickDebris(x, y, LIGHT_BRICK_DEBRIS_SPEED_X, LIGHT_BRICK_DEBRIS_SPEED_Y));
	objects.push_back(new CLightBrickDebris(x, y, -LIGHT_BRICK_DEBRIS_SPEED_X, LIGHT_BRICK_DEBRIS_SPEED_Y));
	this->Delete();
}