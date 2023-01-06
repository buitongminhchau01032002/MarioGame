#pragma once
#include "HiddenBlock.h"
class CHiddenBlockKoopa :
    public CHiddenBlock
{
public:
    CHiddenBlockKoopa(int lCell, int tCell, int rCell, int bCell, int isBlockingLeft, int  isBlockingTop, int isBlockingRight, int isBlockingBottom)
        :CHiddenBlock(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom) {
    };
    int IsOnlyBlockId() {
        return 3;
    }

};
