#include "Koopa.h"
#include "Goomba.h"
#include "QuestionBox.h"
#include "QuestionBoxMushroom.h"
#include "QuestionBoxCoin.h"
#include "PlayScene.h"

CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	this->ay = KOOPA_GRAVITY;
	vx = -KOOPA_WALKING_SPEED;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::MoveInSleep(int direction)
{
	state = KOOPA_STATE_SLEEP;
	vx = KOOPA_SLEEP_SPEED * direction;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_SLEEP || state == KOOPA_STATE_SLEEPING)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SLEEP / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SLEEP;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CQuestionBox*>(e->obj))
		OnCollisionWithQuestionBox(e);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;

	vy += ay * dt;

	if (state == KOOPA_STATE_SLEEPING) {
		vx = 0;
		if (GetTickCount64() - sleepStart > KOOPA_SLEEP_DURATION)
		{
			sleepStart = 0;
			SetState(KOOPA_STATE_WALKING);
		}
	}

	if (state == KOOPA_STATE_WALKING)
		CCollision::GetInstance()->Process(this, dt, coObjects, 3);
	else
		CCollision::GetInstance()->Process(this, dt, coObjects);

}


void CKoopa::Render()
{
	int aniId = ID_ANI_KOOPA_WALKING_LEFT;
	if (vx > 0) {
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
	}
	if (state == KOOPA_STATE_SLEEP)
		aniId = ID_ANI_KOOPA_SLEEP;
	if (state == KOOPA_STATE_SLEEPING)
		aniId = ID_ANI_KOOPA_SLEEPING;

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

/////////////

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SLEEP) {
		CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
		goomba->SetState(GOOMBA_STATE_DIE);
	}
}

void CKoopa::OnCollisionWithQuestionBox(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SLEEP) {
		CQuestionBox* questionBox = dynamic_cast<CQuestionBoxMushroom*>(e->obj);
		if (questionBox) {
			questionBox->Unbox();
		}
		questionBox = dynamic_cast<CQuestionBoxCoin*>(e->obj);
		if (questionBox) {
			questionBox->Unbox();
		}
	}
}

void CKoopa::SetState(int state)
{
	if (state == KOOPA_STATE_SLEEPING) {
		sleepStart = GetTickCount64();
	}
	if (state == KOOPA_STATE_WALKING) {
		if (this->state != KOOPA_STATE_WALKING) {
			y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SLEEP);
		}
		vx = -KOOPA_WALKING_SPEED;
	}
	CGameObject::SetState(state);
}