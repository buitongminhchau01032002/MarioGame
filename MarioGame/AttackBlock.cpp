#include "AttackBlock.h"

void CAttackBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - ATTACK_BLOCK_WIDTH / 2;
    top = y - ATTACK_BLOCK_HEIGHT;
    right = left + ATTACK_BLOCK_WIDTH;
    bottom = top + ATTACK_BLOCK_HEIGHT;
}
void CAttackBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - timer > ATTACK_BLOCK_DURATION) {
        this->Delete();
    }
}
void CAttackBlock::Render()
{
    //RenderBoundingBox();
}