#include "QuestionBox.h"

void CQuestionBox::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_QUESTION_BOX)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTION_BOX_BBOX_WIDTH / 2 + 1;
	t = y - QUESTION_BOX_BBOX_HEIGHT / 2 + 1;
	r = l + QUESTION_BOX_BBOX_WIDTH;
	b = t + QUESTION_BOX_BBOX_HEIGHT;
}