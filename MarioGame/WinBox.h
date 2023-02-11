#pragma once
#include "GameObject.h"

#define WIN_BOX_BBOX_WIDTH 16
#define WIN_BOX_BBOX_HEIGHT 16

#define WIN_BOX_SPEED 3.5f
#define WIN_BOX_DURATION 500
#define WIN_BOX_STEP_TIME 200

#define WIN_BOX_ITEM_STAR 1
#define WIN_BOX_ITEM_MUSHROOM 2
#define WIN_BOX_ITEM_FLOWER 3

#define ID_ANI_WIN_BOX -70000


class CWinBox : public CGameObject {
protected:
	int item = WIN_BOX_ITEM_STAR;
	ULONGLONG changItemStart;
	bool isUnboxed;
	ULONGLONG removeStart;
public:
	CWinBox(int x, int y) :CGameObject(x, y) {
		changItemStart = GetTickCount64();
		isUnboxed = false;
		removeStart = 0;
	};
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - WIN_BOX_BBOX_WIDTH / 2;
		t = y - WIN_BOX_BBOX_HEIGHT / 2;
		r = l + WIN_BOX_BBOX_WIDTH;
		b = t + WIN_BOX_BBOX_HEIGHT;
	};
	int IsBlocking() { return true; }
	int Unbox();
	int GetItem() {
		return item;
	}
};

