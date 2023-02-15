#pragma once
#include "GameObject.h"

#define TUNNEL_OUT_WIDTH 20
#define TUNNEL_OUT_HEIGHT 32

class CTunnelOut : public CGameObject {
	int ny;
	int cameraId;
	int id;
public:
	CTunnelOut(float x, float y, int id, int ny, int cameraId) :CGameObject(x, y) {
		this->ny = ny;
		this->cameraId = cameraId;
		this->id = id;
	};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = x - TUNNEL_OUT_WIDTH / 2;
		top = y - TUNNEL_OUT_HEIGHT / 2 + ny;
		right = left + TUNNEL_OUT_WIDTH;
		bottom = top + TUNNEL_OUT_HEIGHT;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; };

	int GetNy() { return ny; }
	int GetCameraId() { return cameraId; }
	int GetId() { return id; }
};

