#pragma once
#include "GameObject.h"

#define MARIO_MAP_STATE_NONE 0
#define MARIO_MAP_STATE_MOVING 1

class CMarioMap : public CGameObject
{
	int xCell;
	int yCell;
	int tileSize = 16;
	float v = 0.1f;
	int state = MARIO_MAP_STATE_NONE;

	int xCellTo;
	int yCellTo;
public: 
	CMarioMap(int xCell, int yCell);
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2);
		y = float(yTile * tileSize + tileSize / 2);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	};
	int IsBlocking() { return 0; }

	void GoToCell(int xCellTo, int yCellTo) {
		if (state == MARIO_MAP_STATE_MOVING)
			return;
		state = MARIO_MAP_STATE_MOVING;
		this->xCellTo = xCellTo;
		this->yCellTo = yCellTo;
	}
	int GetXCell() { return xCell; }
	int GetYCell() { return yCell; }
};

class CGateConnection {
	int xCell;
	int yCell;
	bool isBlocking;
	bool isNode;
	int tileSize = 16;
public:
	CGateConnection(int xCell, int yCell, bool isBlocking, bool isNode) {
		this->xCell = xCell;
		this->yCell = yCell;
		this->isBlocking = isBlocking;
		this->isNode = isNode;
	}
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2);
		y = float(yTile * tileSize + tileSize / 2);
	}
	int GetXCell() { return xCell; }
	int GetYCell() { return yCell; }
	bool IsBlocking() { return isBlocking; }
	bool IsNode() { return isNode; }
};