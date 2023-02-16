#include "ChomperBullet.h"

CChomperBullet::CChomperBullet(float x, float y, float dx, float dy) :CGameObject(x, y) {
    float k = CHOMPER_BULLET_SPEED / sqrt(dx * dx + dy * dy);
    vx = k * dx;
    vy = k * dy;
    duration = GetTickCount64();
}
void CChomperBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - CHOMPER_BULLET_BBOX_WIDTH / 2;
    top = y - CHOMPER_BULLET_BBOX_HEIGHT / 2;
    right = left + CHOMPER_BULLET_BBOX_WIDTH;
    bottom = top + CHOMPER_BULLET_BBOX_HEIGHT;
}
void CChomperBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - duration > CHOMPER_BULLET_DURATION) this->Delete();
    x += vx * dt;
    y += vy * dt;
}
void CChomperBullet::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_CHOMPER_BULLET)->Render(x, y);
    //RenderBoundingBox();
}

