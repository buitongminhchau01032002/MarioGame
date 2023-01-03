#pragma once

#include "GameObject.h"

#define BLOCK_SIZE 16

class CHiddenBlock : public CGameObject {
private:
	int lCell, tCell, rCell, bCell;
public:
	CHiddenBlock(int lCell, int tCell, int rCell, int bCell);
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

