#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTION_BOX 60000
#define QUESTION_BOX_WIDTH 16
#define QUESTION_BOX_BBOX_WIDTH 16
#define QUESTION_BOX_BBOX_HEIGHT 16

class CQuestionBox : public CGameObject
{
public:
	CQuestionBox(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

