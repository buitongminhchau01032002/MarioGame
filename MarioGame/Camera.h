#pragma once
#include "GameObject.h"
class CCamera
{
private:
	LPGAMEOBJECT following;
	float x;
	float y;
	int width;
	int height;
	float followingOffsetLeft;
	float followingOffsetTop;
	float followingOffsetRight;
	float followingOffsetBottom;
	float limitLeft;
	float limitTop;
	float limitRight;
	float limitBottom;
	float centerOffsetX;
	float centerOffsetY;
	int id;
public:
	CCamera(float x, float y, int width, int height, LPGAMEOBJECT following, 
		float followingOffsetLeft, float followingOffsetTop, float followingOffsetRight, float followingOffsetBottom,
		float limitLeft, float limitTop, float limitRight, float limitBottom,
		float centerOffsetX, float centerOffsetY, int id = 0);
	void SetLimit(float limitLeft, float limitTop, float limitRight, float limitBottom);
	void SetFollowing(LPGAMEOBJECT following) {
		this->following = following;
	}
	void Update();
	void GetPos(float& x, float& y) {
		x = this->x;
		y = this->y;
	}
	int GetId() { return id; }
};

typedef CCamera* LPCAMERA;