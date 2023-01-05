#pragma once
#include "QuestionBox.h"
class CQuestionBoxCoin :
    public CQuestionBox
{
public:
	CQuestionBoxCoin(float x, float y) : CQuestionBox(x, y) {}
	void Unbox();
};

