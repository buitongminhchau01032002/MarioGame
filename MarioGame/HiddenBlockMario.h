#pragma once
#include "debug.h"
#include "HiddenBlock.h"
class CHiddenBlockMario :
    public CHiddenBlock
{ 
public:
    CHiddenBlockMario(int lCell, int tCell, int rCell, int bCell, int isBlockingLeft, int  isBlockingTop, int isBlockingRight, int isBlockingBottom)
        :CHiddenBlock(lCell, tCell, rCell, bCell, isBlockingLeft, isBlockingTop, isBlockingRight, isBlockingBottom) {
    };
    int IsOnlyBlockId() {
        return 0; 
    } // only block id 0 (mario)
};

