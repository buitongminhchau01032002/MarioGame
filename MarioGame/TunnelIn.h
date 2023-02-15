#pragma once
#include "GameObject.h"

#define TUNNEL_IN_WIDTH 24
#define TUNNEL_IN_HEIGHT 32

class CTunnelIn : public CGameObject {
	int ny;
	int tunnelOutId;
	int id;
	int spriteId;
public:
	CTunnelIn(float x, float y, int id, int ny, int tunnelOutId, int spriteId) :CGameObject(x, y) {
		this->ny = ny;
		this->tunnelOutId = tunnelOutId;
		this->id = id;
		this->spriteId = spriteId;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x - TUNNEL_IN_WIDTH / 2;
		top = y - TUNNEL_IN_HEIGHT / 2 - ny*2;
		right = left + TUNNEL_IN_WIDTH;
		bottom = top + TUNNEL_IN_HEIGHT;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	bool IsCanGoTo();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; };
};

