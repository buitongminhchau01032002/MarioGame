#pragma once
#include "GameObject.h"
#include <vector>
using namespace std;

#define MARIO_MAP_SPEED 0.15f

#define MARIO_MAP_STATE_NONE 0
#define MARIO_MAP_STATE_MOVING 1

#define ID_ANI_MARIO_MAP_SMALL 90000
#define ID_ANI_MARIO_MAP_BIG 91000
#define ID_ANI_MARIO_MAP_CAT 92000

#define ID_SPRITE_GATE_COMPLETED 100000

class CGate:CGameObject {
	int xCell;
	int yCell;
	bool isCompleted;
	int sceneId;
	int tileSize = 16;
public:
	CGate(int xCell, int yCell, bool isCompleted, int sceneId) {
		this->xCell = xCell;
		this->yCell = yCell;
		this->isCompleted = isCompleted;
		this->sceneId = sceneId;
		GetObjectXY(xCell, yCell, x, y);
	}
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2);
		y = float(yTile * tileSize + tileSize / 2);
	}
	int GetXCell() { return xCell; }
	int GetYCell() { return yCell; }
	bool IsCompleted() { return isCompleted; }
	void SetCompleted(bool completed) { isCompleted = completed; }
	int GetSceneId() { return sceneId; }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = 0;
		t = 0;
		r = 0;
		b = 0;
	};
	int IsBlocking() { return 0; }
};

class CGateConnection {
	int xCell;
	int yCell;
	bool isBlocking;
	bool isNode;
	bool blockingLeft;
	bool blockingRight;
	bool blockingTop;
	bool blockingBottom;
	int tileSize = 16;
public:
	CGateConnection(int xCell, int yCell, bool isBlocking, bool isNode, bool blockingLeft = false, bool blockingTop = false, bool blockingRight = false, bool blockinBottom = false) {
		this->xCell = xCell;
		this->yCell = yCell;
		this->isBlocking = isBlocking;
		this->isNode = isNode;
		this->blockingBottom = blockinBottom;
		this->blockingLeft = blockingLeft;
		this->blockingRight = blockingRight;
		this->blockingTop = blockingTop;
	}
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2);
		y = float(yTile * tileSize + tileSize / 2);
	}
	int GetXCell() { return xCell; }
	int GetYCell() { return yCell; }
	bool IsBlocking() { return isBlocking; }
	bool IsBlockingLeft() { return blockingLeft; }
	bool IsBlockingRight() { return blockingRight; }
	bool IsBlockingTop() { return blockingTop; }
	bool IsBlockingBottom() { return blockingBottom; }
	bool IsNode() { return isNode; }
};



class CMarioMap : public CGameObject
{
	int xCell;
	int yCell;
	int tileSize = 16;
	float v = MARIO_MAP_SPEED;
	int state = MARIO_MAP_STATE_NONE;

	int xCellTo;
	int yCellTo;

public: 
	CMarioMap(int xCell, int yCell);
	void GetObjectXY(int xTile, int yTile, float& x, float& y) {
		x = float(xTile * tileSize + tileSize / 2) + 1;
		y = float(yTile * tileSize + tileSize / 2) - 1;
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
	void GoRight();
	void GoLeft();
	void GoDown();
	void GoUp();
	int GetXCell() { return xCell; }
	int GetYCell() { return yCell; }
};
