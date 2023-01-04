#pragma once

#include "GameObject.h"

#define BLOCK_SIZE 16

class CHiddenBlock : public CGameObject {
private:
	int lCell, tCell, rCell, bCell;
	int isBlockingTop;
	int isBlockingBottom;
	int isBlockingLeft;
	int isBlockingRight;
public:
	CHiddenBlock(int lCell, int tCell, int rCell, int bCell, int isBlockingLeft, int isBlockingTop, int isBlockingRight, int isBlockingBottom);
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int IsBlockingTop() { return isBlockingTop; }
	int IsBlockingBottom() { return isBlockingBottom; }
	int IsBlockingLeft() { return isBlockingLeft; }
	int IsBlockingRight() { return isBlockingRight; }
};

