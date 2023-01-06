#pragma once
#include "HiddenBlock.h"
class CHiddenBlockGoomba :
    public CHiddenBlock
{
public:
    CHiddenBlockGoomba(int lCell, int tCell, int rCell, int bCell, int isBlockingLeft, int  isBlockingTop, int isBlockingRight, int isBlockingBottom)
        :CHiddenBlock(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom) {
    };
    int IsOnlyBlockId() {
        return 2;
    }
};

