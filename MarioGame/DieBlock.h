#pragma once
#include "HiddenBlock.h"
class CDieBlock :
    public CHiddenBlock
{
public:
    CDieBlock(int lCell, int tCell, int rCell, int bCell, int isBlockingLeft, int  isBlockingTop, int isBlockingRight, int isBlockingBottom)
        :CHiddenBlock(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom) {
    };
};

