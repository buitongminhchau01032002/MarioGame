#include "Koopa.h"
#include "Goomba.h"
#include "QuestionBox.h"
#include "QuestionBoxMushroom.h"
#include "QuestionBoxCoin.h"
#include "PlayScene.h"
#include "Mario.h"
#include "AttackBlock.h"
#include "LightBrick.h"

CKoopa::CKoopa(float x, float y, int type) :CGameObject(x, y)
{
	this->ay = KOOPA_GRAVITY;
	this->type = type;
	vx = -KOOPA_WALKING_SPEED;
	if (type != KOOPA_TYPE_FLY) {
		SetState(KOOPA_STATE_WALKING);
	}
	else {
		SetState(KOOPA_STATE_FLY);
	}
	//SetState(KOOPA_STATE_CARRIED);
}

void CKoopa::MoveInSleep(int direction)
{
	SetState(KOOPA_STATE_SLEEP);
	vx = KOOPA_SLEEP_SPEED * direction;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_SLEEPING || state == KOOPA_STATE_CARRIED) {
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SLEEP / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SLEEP;
	}
	else if (state == KOOPA_STATE_SLEEP)
	{
		left = x - KOOPA_BBOX_WIDTH / 2 + 2;
		top = y;
		right = left + KOOPA_BBOX_WIDTH - 4;
		bottom = y - KOOPA_BBOX_HEIGHT_SLEEP / 2 + KOOPA_BBOX_HEIGHT_SLEEP;
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
	if (e->ny != 0 && state != KOOPA_STATE_FLY)
	{
		vy = 0;
	}
	else if ((e->obj->IsBlocking() || e->obj->IsBlockingTop()) && e->ny < 0) {
		// fly
		vy = -KOOPA_FLY_SPEED;
	}
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CQuestionBox*>(e->obj))
		OnCollisionWithQuestionBox(e);
	else if (dynamic_cast<CLightBrick*>(e->obj))
		OnCollisionWithLightBrick(e);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;

	if (state == KOOPA_STATE_CARRIED) {
		CMario* mario = (CMario*)s->GetPlayer();
		if (mario->GetState() != MARIO_STATE_CARRY) {
			MoveInSleep(mario->Getnx());
			return;
		}
		float xMario, yMario;
		mario->GetPosition(xMario, yMario);
		int nxMario = mario->Getnx();
		int marioLevel = mario->GetLevel();

		if (marioLevel != MARIO_LEVEL_SMALL) {
			x = xMario + nxMario * (MARIO_BIG_BBOX_WIDTH / 2.0f + KOOPA_BBOX_WIDTH / 2.0f - 4);
			y = yMario+4;
		}
		else {
			x = xMario + nxMario * (MARIO_SMALL_BBOX_WIDTH / 2.0f + KOOPA_BBOX_WIDTH / 2.0f - 3);
			y = yMario;
		}

		if (GetTickCount64() - sleepStart > KOOPA_SLEEP_DURATION)
		{
			sleepStart = 0;
			SetState(KOOPA_STATE_WALKING);
		}
		return;
	}

	vy += ay * dt;
	if (state != KOOPA_STATE_SLEEP) {
		vy = KOOPA_SPEED_SLEEP_Y;
	}

	if (state == KOOPA_STATE_SLEEPING) {
		vx = 0;
		if (GetTickCount64() - sleepStart > KOOPA_SLEEP_DURATION)
		{
			sleepStart = 0;
			SetState(KOOPA_STATE_WALKING);
		}
	}

	if (state != KOOPA_STATE_SLEEP) {
		// Check overlap attack block, NOT collision
		vector<LPGAMEOBJECT>& objects = s->GetObjects();
		int index = -1;
		for (int i = 0; i < objects.size(); i++) {
			if (dynamic_cast<CAttackBlock*>(objects[i])) {
				float attack_l, attack_t, attack_r, attack_b;
				float koopa_l, koopa_t, koopa_r, koopa_b;
				this->GetBoundingBox(koopa_l, koopa_t, koopa_r, koopa_b);
				objects[i]->GetBoundingBox(attack_l, attack_t, attack_r, attack_b);
				if (attack_l < koopa_r && attack_r > koopa_l && attack_b > koopa_t && attack_t < koopa_b) {
					this->SetState(KOOPA_STATE_SLEEPING);
					vy = -KOOPA_DEFLECT_SPEED;
					CMario* mario = (CMario*)s->GetPlayer();
					vx = mario->Getnx() * KOOPA_WALKING_SPEED*2;
					break;
				}
			}
		}
	}
	
	if (state == KOOPA_STATE_WALKING)
		CCollision::GetInstance()->Process(this, dt, coObjects, 3);
	else
		CCollision::GetInstance()->Process(this, dt, coObjects);

}


void CKoopa::Render()
{
	int aniId;
	if (state == KOOPA_STATE_FLY) {
		if (vx > 0)
			aniId = ID_ANI_KOOPA_FLY_WALKING_RIGHT;
		else
			aniId = ID_ANI_KOOPA_FLY_WALKING_LEFT;
	}
	else if (state == KOOPA_STATE_WALKING) {
		if (type == KOOPA_TYPE_FLY || type == KOOPA_TYPE_GREEN) {
			if (vx > 0)
				aniId = ID_ANI_KOOPA_GREEN_WALKING_RIGHT;
			else
				aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;
		}
		else {
			if (vx > 0)
				aniId = ID_ANI_KOOPA_RED_WALKING_RIGHT;
			else
				aniId = ID_ANI_KOOPA_RED_WALKING_LEFT;
		}
	}
	else if (state == KOOPA_STATE_SLEEP) {
		if (type == KOOPA_TYPE_FLY || type == KOOPA_TYPE_GREEN) {
			aniId = ID_ANI_KOOPA_GREEN_SLEEP;
		}
		else {
			aniId = ID_ANI_KOOPA_RED_SLEEP;
		}
	}
	else if (state == KOOPA_STATE_SLEEPING || state == KOOPA_STATE_CARRIED) {
		if (type == KOOPA_TYPE_FLY || type == KOOPA_TYPE_GREEN) {
			aniId = ID_ANI_KOOPA_GREEN_SLEEPING;
		}
		else {
			aniId = ID_ANI_KOOPA_RED_SLEEPING;
		}
	}
	else {
		////// 
		if (type == KOOPA_TYPE_FLY || type == KOOPA_TYPE_GREEN) {
			if (vx > 0)
				aniId = ID_ANI_KOOPA_GREEN_WALKING_RIGHT;
			else
				aniId = ID_ANI_KOOPA_GREEN_WALKING_LEFT;
		}
		else {
			if (vx > 0)
				aniId = ID_ANI_KOOPA_RED_WALKING_RIGHT;
			else
				aniId = ID_ANI_KOOPA_RED_WALKING_LEFT;
		}
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
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

void CKoopa::OnCollisionWithLightBrick(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_SLEEP && e->nx != 0) {
		CLightBrick* brick = dynamic_cast<CLightBrick*>(e->obj);
		if (brick) {
			brick->Break();
		}
	}
}

void CKoopa::SetState(int state)
{
	if (state == KOOPA_STATE_SLEEPING) {
		sleepStart = GetTickCount64();
		ay = KOOPA_GRAVITY;
	}
	else if (state == KOOPA_STATE_WALKING) {
		if (this->state != KOOPA_STATE_WALKING) {
			y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_SLEEP);
		}
		vx = -KOOPA_WALKING_SPEED;
		ay = KOOPA_GRAVITY;
	} else if (state == KOOPA_STATE_CARRIED) {
		sleepStart = GetTickCount64();
		ay = KOOPA_GRAVITY;
	} if (state == KOOPA_STATE_FLY) {
		ay = KOOPA_GRAVITY_FLY;
	} if (state == KOOPA_STATE_SLEEP) {
		vy = KOOPA_SPEED_SLEEP_Y;
	}
	CGameObject::SetState(state);
}