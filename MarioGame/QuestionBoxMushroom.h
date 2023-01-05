#pragma once
#include "QuestionBox.h"
class CQuestionBoxMushroom :
    public CQuestionBox
{
public:
    CQuestionBoxMushroom(float x, float y) : CQuestionBox(x, y) {}
    void Unbox();
};

