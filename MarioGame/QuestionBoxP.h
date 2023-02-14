#pragma once
#include "QuestionBox.h"
class CQuestionBoxP :
    public CQuestionBox
{
public:
    CQuestionBoxP(float x, float y) : CQuestionBox(x, y) {}
    void Render();
    void Unbox();
};

