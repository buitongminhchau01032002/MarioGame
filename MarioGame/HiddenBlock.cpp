#include "HiddenBlock.h"
#include "debug.h"

CHiddenBlock::CHiddenBlock(int lCell, int tCell, int rCell, int bCell) {
	this->lCell = lCell;
	this->tCell = tCell;
	this->rCell = rCell;
	this->bCell = bCell;
	float l = float(lCell * BLOCK_SIZE);
	float t = float(tCell * BLOCK_SIZE);
	float r = float(rCell * BLOCK_SIZE + BLOCK_SIZE);
	float b = float(bCell * BLOCK_SIZE + BLOCK_SIZE);
	x = l + (r - l) / 2;
	y = t + (b - t) / 2;
	DebugOut(L"x = %f, y = %f", x, y);
	DebugOut(L"l = %f, t = %f, r = %f, b = %f", l, t, r, b);
}

void CHiddenBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = float(lCell * BLOCK_SIZE);
	t = float(tCell * BLOCK_SIZE);
	r = float(rCell * BLOCK_SIZE + BLOCK_SIZE);
	b = float(bCell * BLOCK_SIZE + BLOCK_SIZE);
}

void CHiddenBlock::Render()
{
	RenderBoundingBox();
}