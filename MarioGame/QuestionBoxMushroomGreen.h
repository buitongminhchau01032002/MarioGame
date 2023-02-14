#pragma once
#include "QuestionBox.h"
class CQuestionBoxMushroomGreen :
    public CQuestionBox
{
public:
    CQuestionBoxMushroomGreen(float x, float y) : CQuestionBox(x, y) { }
    void Render();
    void Unbox();
};

