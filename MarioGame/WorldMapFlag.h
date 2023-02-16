#pragma once

#include "GameObject.h"

#define ID_ANI_WORLDMAP_FLAG -999996

class CWorldMapFlag : public CGameObject {
	int xCell, yCell;
	int blockSize = 16;
public:
	CWorldMapFlag(int xCell, int yCell) {
		this->xCell = xCell;
		this->yCell = yCell;
		x = float(xCell * blockSize + blockSize / 2 + 1);
		y = float(yCell * blockSize + blockSize / 2 - 1);
	}
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	};
	int IsCollidable() { return false; }
	int IsBlocking() { return false; }
};

