#include "QuestionBox.h"

void CQuestionBox::Unbox()
{
	if (state == QUESTION_BOX_STATE_ORIGIN) {
		unbox_start = GetTickCount64();
		state = QUESTION_BOX_STATE_UNBOXING;
	}
}

void CQuestionBox::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (state == QUESTION_BOX_STATE_ORIGIN)
		animations->Get(ID_ANI_QUESTION_BOX)->Render(x, y);
	else
		animations->Get(ID_ANI_QUESTION_BOX+1)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == QUESTION_BOX_STATE_ORIGIN || state == QUESTION_BOX_STATE_UNBOXED) return;
	if (GetTickCount64() - unbox_start < QUESTION_BOX_UNBOX_DURATION / 2) {
		y -= dt * QUESTION_BOX_UNBOX_SPEED;
	}
	else if (GetTickCount64() - unbox_start < QUESTION_BOX_UNBOX_DURATION) {
		y += dt * QUESTION_BOX_UNBOX_SPEED;
	}
	else {
		y = originY;
		state = QUESTION_BOX_STATE_UNBOXED;
	}
}

void CQuestionBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BOX_BBOX_WIDTH / 2 + 1;
	t = y - QUESTION_BOX_BBOX_HEIGHT / 2 + 1;
	r = l + QUESTION_BOX_BBOX_WIDTH;
	b = t + QUESTION_BOX_BBOX_HEIGHT;
}