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
	DebugOutTitle(L"stateY: %d, vy: %f", stateY, vy);

	// HANDLE running duration
	if (stateX == MARIO_STATE_X_RUNNING) {
		runningDuration += dt;
		if (runningDuration > MARIO_RUNNING_DURATION_MAX) runningDuration = MARIO_RUNNING_DURATION_MAX;
	}
	else {
		runningDuration -= dt;
		if (runningDuration < 0) runningDuration = 0;
	}
	

	// HANDLE vy
	vy += ay * dt;
	if (vy > maxVy) vy = maxVy;
	if (stateY == MARIO_STATE_Y_JUMPING && vy >= 0) {
		SetStateY(MARIO_STATE_Y_FALLING);
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
	}
	else {
		vx += ax * dt;
	}
	if (abs(vx) > abs(maxVx)) vx = maxVx;


	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// Slow falling timer
	if (stateY == MARIO_STATE_Y_SLOWFALLING) {
		if (GetTickCount64() - slowFallingStart > MARIO_SLOWFALLING_BREAK_TIME)
		{
			slowFallingStart = 0;
			SetStateY(MARIO_STATE_Y_FALLING);
		}
	}

	// flying timer
	if (stateY == MARIO_STATE_Y_FLYING) {
		if (GetTickCount64() - flyingStart > MARIO_FLYING_DURATION)
		{
			flyingStart = 0;
			SetStateY(MARIO_STATE_Y_FALLING);
		}
	}

	if (state == MARIO_STATE_DIE) {
		CGame::GetInstance()->GetCamera()->SetFollowing(NULL);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects, 0);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	if (vy > MARIO_SPEED_TO_FALL && (level != MARIO_LEVEL_CAT || stateY != MARIO_STATE_Y_FLYING ))
		SetStateY(MARIO_STATE_Y_FALLING);
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0)
	{
		if (e->ny == -1 && (e->obj->IsBlocking() || e->obj->IsBlockingTop())) {
			vy = 0;
			SetStateY(MARIO_STATE_Y_GROUND);
		}
		if (e->ny == 1 && (e->obj->IsBlocking() || e->obj->IsBlockingBottom())) {
			vy = 0;
		}
	}
	else 
	if (e->nx != 0 && (e->obj->IsBlocking() || e->obj->IsBlockingLeft() || e->obj->IsBlockingRight()))
	{
		vx = 0;
		SetStateX(MARIO_STATE_X_IDLE);
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
// Get animation ID for Mario
//
int CMario::GetAniIdSmall()
{
	// IN AIR
	// jumping
	if (stateY == MARIO_STATE_Y_JUMPING) {
		if (IsCanFly()) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else return ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		if (nx > 0) return ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
		else return ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
	}
	// falling
	if (stateY == MARIO_STATE_Y_FALLING) {
		if (nx > 0) return ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
		else return ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
	}

	
	// GROUNDING
	// idle
	if (stateX == MARIO_STATE_X_IDLE) {
		if (nx > 0) return ID_ANI_MARIO_SMALL_IDLE_RIGHT;
		else return ID_ANI_MARIO_SMALL_IDLE_LEFT;
	} 
	// bracing
	if (stateX == MARIO_STATE_X_BRACING) {
		if (nx > 0) return ID_ANI_MARIO_SMALL_BRACE_RIGHT;
		else return ID_ANI_MARIO_SMALL_BRACE_LEFT;
	}
	// can fly
	if (stateX == MARIO_STATE_X_RUNNING && IsCanFly()) {
		if (nx > 0) return ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
		else return ID_ANI_MARIO_SMALL_RUNNING_LEFT;
	}
	// walking and running
	if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING || stateX == MARIO_STATE_X_RUNNING) {
		if (nx > 0) return ID_ANI_MARIO_SMALL_WALKING_RIGHT;
		else return ID_ANI_MARIO_SMALL_WALKING_LEFT;
	}

	return ID_ANI_MARIO_SMALL_IDLE_RIGHT;
}

int CMario::GetAniIdBig()
{
	// IN AIR
	// jumping
	if (stateY == MARIO_STATE_Y_JUMPING) {
		if (IsCanFly()) {
			if (nx > 0) return ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else return ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		if (nx > 0) return ID_ANI_MARIO_JUMP_WALK_RIGHT;
		else return ID_ANI_MARIO_JUMP_WALK_LEFT;
	}
	// falling
	if (stateY == MARIO_STATE_Y_FALLING) {
		if (nx > 0) return ID_ANI_MARIO_JUMP_WALK_RIGHT;
		else return ID_ANI_MARIO_JUMP_WALK_LEFT;
	}

	// GROUNDING
	// idle
	if (stateX == MARIO_STATE_X_IDLE) {
		if (nx > 0) return ID_ANI_MARIO_IDLE_RIGHT;
		else return ID_ANI_MARIO_IDLE_LEFT;
	}
	// bracing
	if (stateX == MARIO_STATE_X_BRACING) {
		if (nx > 0) return ID_ANI_MARIO_BRACE_RIGHT;
		else return ID_ANI_MARIO_BRACE_LEFT;
	}
	// can fly
	if (stateX == MARIO_STATE_X_RUNNING && IsCanFly()) {
		if (nx > 0) return ID_ANI_MARIO_RUNNING_RIGHT;
		else return ID_ANI_MARIO_RUNNING_LEFT;
	}
	// walking and running
	if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING || stateX == MARIO_STATE_X_RUNNING) {
		if (nx > 0) return ID_ANI_MARIO_WALKING_RIGHT;
		else return ID_ANI_MARIO_WALKING_LEFT;
	}

	return ID_ANI_MARIO_IDLE_RIGHT;
}

int CMario::GetAniIdCat()
{
	// IN AIR
	// jumping
	if (stateY == MARIO_STATE_Y_JUMPING) {
		if (IsCanFly()) {
			if (nx > 0) return ID_ANI_MARIO_CAT_JUMP_RUN_RIGHT;
			else return ID_ANI_MARIO_CAT_JUMP_RUN_LEFT;
		}
		if (nx > 0) return ID_ANI_MARIO_CAT_JUMP_WALK_RIGHT;
		else return ID_ANI_MARIO_CAT_JUMP_WALK_LEFT;
	}
	// falling
	if (stateY == MARIO_STATE_Y_FALLING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_JUMP_WALK_RIGHT;
		else return ID_ANI_MARIO_CAT_JUMP_WALK_LEFT;
	}
	// falling show
	if (stateY == MARIO_STATE_Y_SLOWFALLING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_IDLE_RIGHT;
		else return ID_ANI_MARIO_CAT_IDLE_LEFT;
	}
	// flying
	if (stateY == MARIO_STATE_Y_FLYING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_FLYING_RIGHT;
		else return ID_ANI_MARIO_CAT_FLYING_LEFT;
	}

	// GROUNDING
	// idle
	if (stateX == MARIO_STATE_X_IDLE) {
		if (nx > 0) return ID_ANI_MARIO_CAT_IDLE_RIGHT;
		else return ID_ANI_MARIO_CAT_IDLE_LEFT;
	}
	// bracing
	if (stateX == MARIO_STATE_X_BRACING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_BRACE_RIGHT;
		else return ID_ANI_MARIO_CAT_BRACE_LEFT;
	}
	// can fly
	if (stateX == MARIO_STATE_X_RUNNING && IsCanFly()) {
		if (nx > 0) return ID_ANI_MARIO_CAT_RUNNING_RIGHT;
		else return ID_ANI_MARIO_CAT_RUNNING_LEFT;
	}
	// walking and running
	if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING || stateX == MARIO_STATE_X_RUNNING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_WALKING_RIGHT;
		else return ID_ANI_MARIO_CAT_WALKING_LEFT;
	}

	return ID_ANI_MARIO_CAT_IDLE_RIGHT;
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
	case MARIO_STATE_X_RUNNING:
		ax = nx * MARIO_ACCEL_RUN_X;
		maxVx = nx * MARIO_RUNNING_SPEED;
		break;
	case MARIO_STATE_X_WALK_STOPPING:
		ax = nx * MARIO_ACCEL_WALK_X/2;
		break;
	case MARIO_STATE_X_BRACING:
		ax = nx * MARIO_ACCEL_BRACE;
		break;
	default:
		break;
	}
	stateX = state;
}

void CMario::SetStateY(int state)
{
	switch (state)
	{
	case MARIO_STATE_Y_GROUND:
		vy = 0;
		ay = MARIO_GRAVITY;
		break;
	case MARIO_STATE_Y_JUMPING:
		ay = MARIO_GRAVITY;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_Y_FALLING:
		ay = MARIO_GRAVITY;
		maxVy = MARIO_SPEED_Y_MAX;
		break;
	case MARIO_STATE_Y_SLOWFALLING:
		ay = MARIO_GRAVITY;
		maxVy = MARIO_SPEED_Y_SLOW_MAX;
		slowFallingStart = GetTickCount64();
		break;
	case MARIO_STATE_Y_FLYING:
		vy = -MARIO_JUMP_SPEED_Y;
		maxVy = MARIO_SPEED_Y_MAX;
		ay = MARIO_GRAVITY_FLYING;
		flyingStart = GetTickCount64();
		break;
	default:
		break;
	}
	stateY = state;
}