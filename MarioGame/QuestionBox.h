#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"


#define ID_ANI_QUESTION_BOX 60000
#define QUESTION_BOX_WIDTH 16
#define QUESTION_BOX_BBOX_WIDTH 16
#define QUESTION_BOX_BBOX_HEIGHT 16
#define QUESTION_BOX_STATE_ORIGIN 0
#define QUESTION_BOX_STATE_UNBOXING 1
#define QUESTION_BOX_STATE_UNBOXED 2
#define QUESTION_BOX_UNBOX_SPEED 0.2f
#define QUESTION_BOX_UNBOX_DURATION 150


class CQuestionBox : public CGameObject
{
protected:
	ULONGLONG unbox_start;
	float originY;
public:
	CQuestionBox(float x, float y) : CGameObject(x, y) {
		state = QUESTION_BOX_STATE_ORIGIN;
		unbox_start = -1;
		originY = y;
	}
	virtual void Unbox();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

