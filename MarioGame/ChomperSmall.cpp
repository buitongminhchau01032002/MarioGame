#include "ChomperSmall.h"
#include "PlayScene.h"


CChomperSmall::CChomperSmall(float x, float y) :CGameObject(x, y)
{
	originY = y;
	state = CHOMPER_SMALL_STATE_HIDDEN;
	pushTimerStart = GetTickCount64();
}
void CChomperSmall::GetShowBox(float& left, float& top, float& right, float& bottom)
{
	left = x - CHOMPER_SMALL_BBOX_WIDTH / 2;
	top = y - CHOMPER_SMALL_BBOX_HEIGHT / 2;
	right = left + CHOMPER_SMALL_BBOX_WIDTH;
	bottom = originY - 1 - 16;

}

void CChomperSmall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - CHOMPER_SMALL_BBOX_WIDTH / 2;
	top = y - CHOMPER_SMALL_BBOX_HEIGHT / 2;
	right = left + CHOMPER_SMALL_BBOX_WIDTH;
	bottom = top + CHOMPER_SMALL_BBOX_HEIGHT;

}

void CChomperSmall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;
	if (state == CHOMPER_SMALL_STATE_DIE) {
		return;
	}

	if (state == CHOMPER_SMALL_STATE_HIDDEN) {
		y += CHOMPER_SMALL_SHOW_SPEED * dt;
		if (y > originY) {
			y = originY;
		}
		if (GetTickCount64() - pushTimerStart > CHOMPER_SMALL_HIDDEN_TIME) {
			pullTimerStart = GetTickCount64();
			state = CHOMPER_SMALL_STATE_SHOW;
		}
	}
	else if (state == CHOMPER_SMALL_STATE_SHOW) {
		y -= CHOMPER_SMALL_SHOW_SPEED * dt;
		int pushHeight = CHOMPER_SMALL_PUSH_HEIGHT;
		if (y < originY - pushHeight) {
			y = originY - pushHeight;
		}
		if (GetTickCount64() - pullTimerStart > CHOMPER_SMALL_HIDDEN_TIME) {
			pushTimerStart = GetTickCount64();
			state = CHOMPER_SMALL_STATE_HIDDEN;
		}
	}

}

void CChomperSmall::Render()
{
	if (state == CHOMPER_SMALL_STATE_DIE) {
		CAnimations::GetInstance()->Get(ID_ANI_CHOMPER_SMALL_PIPE)->Render(x, originY - 1);
		return;
	}
	int aniId = ID_ANI_CHOMPER_SMALL;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	CAnimations::GetInstance()->Get(ID_ANI_CHOMPER_SMALL_PIPE)->Render(x, originY - 1);
	//RenderBoundingBox();
}