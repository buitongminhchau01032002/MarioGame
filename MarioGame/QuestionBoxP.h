#pragma once
#include "QuestionBox.h"
class CQuestionBoxP :
    public CQuestionBox
{
    int group;
public:
    CQuestionBoxP(float x, float y, int group) : CQuestionBox(x, y) { this->group = group; }
    void Render();
    void Unbox();
};

