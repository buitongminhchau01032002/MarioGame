#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "MyPortal.h"
#include "Scene.h"
#include "PlayScene.h"
#include "QuestionBox.h"
#include "Mushroom.h"
#include "Koopa.h"
#include "GoombaPro.h"
#include "Chomper.h"
#include "ChomperBullet.h"
#include "PlayScene.h"
#include "AttackBlock.h"
#include "DieBlock.h"
#include "Leaf.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	DebugOutTitle(L"state: %d, vy: %f, y: %f", stateX, vy, y);
	if (state != MARIO_STATE_RUNNING) {
		runningDuration = 0;
	}
	else {
		runningDuration += dt;
	}
	if (runningDuration > MARIO_FLY_STARTUP_DURATION)
	{
		canFly = true;
	}
	flyingDuration -= dt;
	if (flyingDuration < 0) { flyingDuration = 0;  }
	
	if (!isFlying) {
		if (flyingDuration > 0) {
			vy = MARIO_FLY_SPEED;
		}
		else {

			vy += ay * dt;
		}
	}
	else {
		maxVx = nx * MARIO_WALKING_SPEED;
		vy = -MARIO_FLY_SPEED;
	}



	// HANDLE vx
	if (stateX == MARIO_STATE_X_WALK_STOPPING && vx != 0.0f) {
		if (vx > 0.0f) {
			vx += -MARIO_ACCEL_WALK_X * dt;
			if (vx < 0.0f) {
				vx = 0.0f;
				SetStateX(MARIO_STATE_X_IDLE);
			}
		} else {
			vx += MARIO_ACCEL_WALK_X * dt;
			if (vx > 0.0f) {
				vx = 0.0f;
				SetStateX(MARIO_STATE_X_IDLE);
			}
		}
	} else {
		vx += ax * dt;
	}
	if (abs(vx) > abs(maxVx)) vx = maxVx;


	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (GetTickCount64() - flyingBreakTimerStart > MARIO_FLY_BREAK_TIME) {
		isFlying = false;
	}

	isOnPlatform = false;

	if (state == MARIO_STATE_DIE) {
		CGame::GetInstance()->GetCamera()->SetFollowing(NULL);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects, 0);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		if (e->ny == -1 && (e->obj->IsBlocking() || e->obj->IsBlockingTop())) {
			vy = 0;
			isOnPlatform = true;
		}
		if (e->ny == 1 && (e->obj->IsBlocking() || e->obj->IsBlockingBottom())) {
			vy = 0;
		}
	}
	else 
	if (e->nx != 0 && (e->obj->IsBlocking() || e->obj->IsBlockingLeft() || e->obj->IsBlockingRight()))
	{
		vx = 0;
	}

	if (dynamic_cast<CGoombaPro*>(e->obj))
		OnCollisionWithGoombaPro(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBox*>(e->obj))
		OnCollisionWithQuestionBox(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CChomperBullet*>(e->obj)) {
		if (untouchable == 0) DecreaseLevel();
	}
	else if (dynamic_cast<CChomper*>(e->obj)) {
		if (untouchable == 0) DecreaseLevel();
	}
	else if (dynamic_cast<CDieBlock*>(e->obj)) {
		SetState(MARIO_STATE_DIE);
	}
	
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				DecreaseLevel();
			}
		}
	}
}

void CMario::OnCollisionWithGoombaPro(LPCOLLISIONEVENT e)
{
	CGoombaPro* goomba = dynamic_cast<CGoombaPro*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			coin += 100;
			if (goomba->GetLevel() == GOOMBA_PRO_LEVEL_NORMAL) {

			goomba->SetState(GOOMBA_STATE_DIE);
			}
			else {

				goomba->SetLevel(GOOMBA_PRO_LEVEL_NORMAL);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				DecreaseLevel();
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin += 100;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBox(LPCOLLISIONEVENT e)
{
	if (e->ny == 1) {
		CQuestionBox* questionBox = (CQuestionBox*)(e->obj);
		questionBox->Unbox();
		coin += 100;
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = (CMushroom*)(e->obj);
	mushroom->Delete();
	IncreaseLevel();
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = (CLeaf*)(e->obj);
	leaf->Delete();
	IncreaseLevel();
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = (CKoopa*)(e->obj);
	if (koopa->GetState() == KOOPA_STATE_WALKING) {
		// jump on top >> sleeping koopa and deflect a bit 
		if (e->ny < 0)
		{
			koopa->SetState(KOOPA_STATE_SLEEPING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else // hit by koopa
		{
			if (untouchable == 0)
			{
				DecreaseLevel();
			}
		}
	} else if (koopa->GetState() == KOOPA_STATE_SLEEPING) {
		float koopaX;
		float koopaY;
		koopa->GetPosition(koopaX, koopaY);
		if (this->x < koopaX) {
			koopa->MoveInSleep(1);
		}
		else {
			koopa->MoveInSleep(-1);
		}
	}
	else if (koopa->GetState() == KOOPA_STATE_SLEEP) {
		if (e->nx != 0)
		{
			if (untouchable == 0)
			{
				DecreaseLevel();
			}
		}
	}
	
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else if (isSitting) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_SIT_RIGHT;
		else
			aniId = ID_ANI_MARIO_SIT_LEFT;
	}
	else { // stand
		if (stateX == MARIO_STATE_X_IDLE) {
			if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
		} else if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING) {
			if (nx > 0) aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
		} else {
			if (nx > 0) aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
			else aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
		}
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdCat()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (!isFlying) {
			if (abs(ax) == MARIO_ACCEL_RUN_X)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_CAT_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_CAT_JUMP_RUN_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_CAT_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_CAT_JUMP_WALK_LEFT;
			}
		}
		else {
			if (nx >= 0)
				aniId = ID_ANI_MARIO_CAT_FLYING_RIGHT;
			else
				aniId = ID_ANI_MARIO_CAT_FLYING_LEFT;
		}
	}
	else {
		if (GetTickCount64()- attackStart < MARIO_ATTACK_EFFECT_DURATION) {
			if (nx > 0) aniId = ID_ANI_MARIO_ATTACK_RIGHT;
			else aniId = ID_ANI_MARIO_ATTACK_LEFT;
		}
		else if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_CAT_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_CAT_SIT_LEFT;
		}
		else {
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_CAT_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_CAT_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_CAT_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X) {
					aniId = ID_ANI_MARIO_CAT_WALKING_RIGHT;
					if (canFly) {
						aniId = ID_ANI_MARIO_CAN_FLY_RIGHT;
					}
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_CAT_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_CAT_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					aniId = ID_ANI_MARIO_CAT_WALKING_LEFT;
					if (canFly) {
						aniId = ID_ANI_MARIO_CAN_FLY_LEFT;
					}
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_CAT_WALKING_LEFT;
			}
		}
	}

		

	if (aniId == -1) aniId = ID_ANI_MARIO_CAT_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_CAT)
		aniId = GetAniIdCat();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING:
		if (isSitting) break;
		maxVx = nx*MARIO_RUNNING_SPEED;
		ax = nx*MARIO_ACCEL_RUN_X;
		break;
	case MARIO_STATE_WALKING:
		if (isSitting) break;
		maxVx = nx*MARIO_WALKING_SPEED;
		ax = nx*MARIO_ACCEL_WALK_X;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		ay = MARIO_GRAVITY;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_FLY:
		isFlying = true;
		flyingBreakTimerStart = GetTickCount64();
		canFly = false;
		break;
	case MARIO_STATE_START_FLY:
		isFlying = true;
		flyingBreakTimerStart = GetTickCount64();
		canFly = false;
		flyingDuration = MARIO_FLYING_DURATION;
		break;
	case MARIO_STATE_ACTTACK:
		attackStart = GetTickCount64();
		LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT>& objects = s->GetObjects();
		objects.push_back(new CAttackBlock(x + nx*(MARIO_BIG_BBOX_WIDTH), y));
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level==MARIO_LEVEL_CAT)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT + 1;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT + 1;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT + 1;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

void CMario::IncreaseLevel() {
	if (level == MARIO_LEVEL_SMALL) SetLevel(MARIO_LEVEL_BIG);
	else if (level == MARIO_LEVEL_BIG) SetLevel(MARIO_LEVEL_CAT);
}

void CMario::DecreaseLevel() {
	isFlying = false;
	if (level == MARIO_LEVEL_SMALL) SetState(MARIO_STATE_DIE);
	else if (level == MARIO_LEVEL_BIG) 
	{
		SetLevel(MARIO_LEVEL_SMALL);
		StartUntouchable();
	}
	else 
	{
		SetLevel(MARIO_LEVEL_BIG);
		StartUntouchable();
	}
}

void CMario::SetStateX(int state)
{
	switch (state)
	{
	case MARIO_STATE_X_IDLE:
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_X_WALKING:
		ax = nx * MARIO_ACCEL_WALK_X;
		maxVx = nx * MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_X_WALK_STOPPING:
		ax = nx * MARIO_ACCEL_WALK_X;
		break;
	case MARIO_STATE_X_BRACING:
		ax = nx * MARIO_ACCEL_BRACE;
		break;
	default:
		break;
	}
	stateX = state;
}
