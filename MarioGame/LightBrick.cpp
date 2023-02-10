#include "LightBrick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Animations.h"
#include "debug.h"

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

	this->Delete();
}