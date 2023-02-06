#include "Camera.h"
#include "debug.h"

CCamera::CCamera(float x, float y, int w, int h, LPGAMEOBJECT following,
	float followingOffsetLeft, float followingOffsetTop, float followingOffsetRight, float followingOffsetBottom,
	float limitLeft, float limitTop, float limitRight, float limitBottom,
	float centerOffsetX, float centerOffsetY) {
	this->following = following;
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
	this->followingOffsetLeft = followingOffsetLeft;
	this->followingOffsetTop = followingOffsetTop;
	this->followingOffsetRight = followingOffsetRight;
	this->followingOffsetBottom = followingOffsetBottom;
	this->limitLeft = limitLeft;
	this->limitTop = limitTop;
	this->limitRight = limitRight;
	this->limitBottom = limitBottom;
	this->centerOffsetX = centerOffsetX;
	this->centerOffsetY = centerOffsetY;
}

void CCamera::SetLimit(float limitLeft, float limitTop, float limitRight, float limitBottom)
{
	this->limitLeft = limitLeft;
	this->limitTop = limitTop;
	this->limitRight = limitRight;
	this->limitBottom = limitBottom;
}

void CCamera::Update()
{
	if (following == NULL) {
		return;
	}
	float followingX, followingY;
	float centerCamX = (float)width / 2 + x + centerOffsetX;
	float centerCamY = (float)height / 2 + y + centerOffsetY;
	following->GetPosition(followingX, followingY);

	// Check following offset
	if (centerCamX - followingX > followingOffsetLeft) {
		x = (followingOffsetLeft + followingX) - (float)width / 2;
	}
	if (centerCamY - followingY > followingOffsetTop) {
		y = (followingOffsetTop + followingY) - (float)height / 2;
	}
	if (followingX - centerCamX > followingOffsetRight) {
		x = (followingX - followingOffsetLeft) - (float)width / 2;
	}
	if (followingY - centerCamY > followingOffsetBottom) {
		y = (followingY - followingOffsetBottom) - (float)height / 2;
	}

	// Check limit
	if (x < limitLeft) x = limitLeft;
	if (x + width > limitRight) x = limitRight - width;
	if (y < limitTop) y = limitTop;
	if (y + height > limitBottom) y = limitBottom - height;
}


