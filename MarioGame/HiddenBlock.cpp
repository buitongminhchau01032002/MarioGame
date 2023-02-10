#include "HiddenBlock.h"
#include "Game.h"
#include "PlayScene.h"
#include "debug.h"

CHiddenBlock::CHiddenBlock(int lCell, int tCell, int rCell, int bCell, int isBlockingLeft, int isBlockingTop, int isBlockingRight, int isBlockingBottom
) {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	this->lCell = lCell;
	this->tCell = tCell;
	this->rCell = rCell;
	this->bCell = bCell;
	float l = float(lCell * blockSize);
	float t = float(tCell * blockSize);
	float r = float(rCell * blockSize + blockSize);
	float b = float(bCell * blockSize + blockSize);
	x = l + (r - l) / 2;
	y = t + (b - t) / 2;
	this->isBlockingTop = isBlockingTop;
	this->isBlockingBottom = isBlockingBottom;
	this->isBlockingRight = isBlockingRight;
	this->isBlockingLeft = isBlockingLeft;
}

void CHiddenBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	l = float(lCell * blockSize);
	t = float(tCell * blockSize);
	r = float(rCell * blockSize + blockSize);
	b = float(bCell * blockSize + blockSize);
}

void CHiddenBlock::Render()
{
	RenderBoundingBox();
}