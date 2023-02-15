#include <algorithm>
#include <fstream>
#include "debug.h"
#include "Utils.h"
#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "MyPortal.h"
#include "Scene.h"
#include "PlayScene.h"
#include "WorldMapScene.h"
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
#include "ChomperSmall.h"
#include "WinBox.h"
#include "P.h"
#include "MushroomGreen.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	DebugOutTitle(L"state: %d, stateX: %d, stateY: %d, vy: %f", state, stateX, stateY, vy);
	if (state == MARIO_STATE_WINNING) {
		if (GetTickCount64() - winStart > MARIO_WIN_DURATION)
		{
			SaveToFile();
			CGame::GetInstance()->InitiateSwitchScene(0);
		}
		untouchable = 1;
		nx = 1;
		SetStateX(MARIO_STATE_X_WALKING);
		SetStateY(MARIO_STATE_Y_FALLING);
		vy += ay * dt;
		vx += ax * dt;
		CCollision::GetInstance()->Process(this, dt, coObjects, 1);
		return;
	}

	if (state == MARIO_STATE_IN_TUNEL) {
		y += tunnelNy* MARIO_TUNNEL_SPEED * dt;
		return;
	}


	// HANDLE running duration
	if (stateX == MARIO_STATE_X_RUNNING && stateY == MARIO_STATE_Y_GROUND) {
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


	// time
	time = GetTickCount64() - timeStart;

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

	// attack timer
	if (state == MARIO_STATE_ATTACK) {
		if (GetTickCount64() - attackStart > MARIO_ATTACK_EFFECT_DURATION)
		{
			attackStart = 0;
			SetState(MARIO_STATE_NONE);
		}
	}

	// die
	if (state == MARIO_STATE_DIE) {
		if (GetTickCount64() - dieStart > MARIO_DIE_DURATION)
		{
			SaveToFile();
			CGame::GetInstance()->InitiateSwitchScene(0);
		}
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
		runningDuration = 0;
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
	else if (dynamic_cast<CMushroomGreen*>(e->obj)) {
		CMushroomGreen* mushroom = (CMushroomGreen*)(e->obj);
		mushroom->Delete();
		CGame::GetInstance()->IncreaseHeart(1);
	}
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CChomperBullet*>(e->obj)) {
		if (untouchable == 0) DecreaseLevel();
	}
	else if (dynamic_cast<CChomper*>(e->obj)) {
		CChomper* chomper = dynamic_cast<CChomper*>(e->obj);
		if (untouchable == 0 && chomper->GetState() != CHOMPER_STATE_DIE) DecreaseLevel();
	}
	else if (dynamic_cast<CChomperSmall*>(e->obj)) {
		CChomperSmall* chomper = dynamic_cast<CChomperSmall*>(e->obj);
		if (untouchable == 0 && chomper->GetState() != CHOMPER_SMALL_STATE_DIE) DecreaseLevel();
	} else if (dynamic_cast<CP*>(e->obj)) {
		CP* p = dynamic_cast<CP*>(e->obj);
		// jump on top P
		if (e->ny < 0 && p->GetState() == P_STATE_SHOW)
		{
			p->Press();
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else if (dynamic_cast<CDieBlock*>(e->obj)) {
		SetState(MARIO_STATE_DIE);
	}
	else if (dynamic_cast<CWinBox*>(e->obj)) {
		CWinBox* box = dynamic_cast<CWinBox*>(e->obj);
		int item = box->Unbox();
		SetState(MARIO_STATE_WINNING);
	}
	
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_STRONG)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			CGame* g = CGame::GetInstance();
			g->IncreaseCoinValue(100);
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_STRONG)
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
		if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_STRONG)
		{
			CGame* g = CGame::GetInstance();
			g->IncreaseCoinValue(100);
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
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_STRONG)
			{
				DecreaseLevel();
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	CGame* g = CGame::GetInstance();
	g->IncreaseCoin(1);
	g->IncreaseCoinValue(100);
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
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = (CMushroom*)(e->obj);
	mushroom->Delete();
	IncreaseLevel();
	CGame* g = CGame::GetInstance();
	g->IncreaseCoinValue(1000);
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* leaf = (CLeaf*)(e->obj);
	leaf->Delete();
	IncreaseLevel();
	CGame* g = CGame::GetInstance();
	g->IncreaseCoinValue(1000);
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = (CKoopa*)(e->obj);
	if (koopa->GetState() == KOOPA_STATE_WALKING || koopa->GetState() == KOOPA_STATE_SLEEP || koopa->GetState() == KOOPA_STATE_FLY) {
		// jump on top >> sleeping koopa and deflect a bit 
		if (e->ny < 0)
		{
			if (koopa->GetState() == KOOPA_STATE_WALKING) {
				koopa->SetState(KOOPA_STATE_SLEEPING);
			}
			else if (koopa->GetState() == KOOPA_STATE_FLY) {
				koopa->SetState(KOOPA_STATE_WALKING);
			}
			else if (koopa->GetState() == KOOPA_STATE_SLEEP) {
				koopa->SetState(KOOPA_STATE_SLEEPING);
			}
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			CGame* g = CGame::GetInstance();
			g->IncreaseCoinValue(100);
		}
		else // hit by koopa
		{
			if (untouchable == 0)
			{
				DecreaseLevel();
			}
		}
	} else if (koopa->GetState() == KOOPA_STATE_SLEEPING) {
		if (isAKeyPress && e->nx != 0 && state != MARIO_STATE_CARRY) {
			// carry
			SetState(MARIO_STATE_CARRY);
			koopa->SetState(KOOPA_STATE_CARRIED);
		}
		else {
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
	if (state == MARIO_STATE_IN_TUNEL) {
		return ID_ANI_MARIO_SMALL_TUNNEL;
	}

	if (state == MARIO_STATE_CARRY) {
		// IN AIR
		// jumping
		if (stateY == MARIO_STATE_Y_JUMPING) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_CARRY_JUMP_RIGHT;
			else return ID_ANI_MARIO_SMALL_CARRY_JUMP_LEFT;
		}
		// falling
		if (stateY == MARIO_STATE_Y_FALLING) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_CARRY_JUMP_RIGHT;
			else return ID_ANI_MARIO_SMALL_CARRY_JUMP_LEFT;
		}


		// GROUNDING
		// idle
		if (stateX == MARIO_STATE_X_IDLE) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_CARRY_IDLE_RIGHT;
			else return ID_ANI_MARIO_SMALL_CARRY_IDLE_LEFT;
		}
		// bracing
		if (stateX == MARIO_STATE_X_BRACING) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_CARRY_IDLE_RIGHT;
			else return ID_ANI_MARIO_SMALL_CARRY_IDLE_LEFT;
		}
		// can fly
		if (stateX == MARIO_STATE_X_RUNNING && IsCanFly()) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_CARRY_RUNNING_RIGHT;
			else return ID_ANI_MARIO_SMALL_CARRY_RUNNING_LEFT;
		}
		// walking and running
		if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING || stateX == MARIO_STATE_X_RUNNING) {
			if (nx > 0) return ID_ANI_MARIO_SMALL_CARRY_WALKING_RIGHT;
			else return ID_ANI_MARIO_SMALL_CARRY_WALKING_LEFT;
		}

	}
	
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
	if (state == MARIO_STATE_IN_TUNEL) {
		return ID_ANI_MARIO_TUNNEL;
	}

	if (state == MARIO_STATE_CARRY) {
		// IN AIR
		// jumping
		if (stateY == MARIO_STATE_Y_JUMPING) {
			if (nx > 0) return ID_ANI_MARIO_CARRY_JUMP_RIGHT;
			else return ID_ANI_MARIO_CARRY_JUMP_LEFT;
		}
		// falling
		if (stateY == MARIO_STATE_Y_FALLING) {
			if (nx > 0) return ID_ANI_MARIO_CARRY_JUMP_RIGHT;
			else return ID_ANI_MARIO_CARRY_JUMP_LEFT;
		}


		// GROUNDING
		// idle
		if (stateX == MARIO_STATE_X_IDLE) {
			if (nx > 0) return ID_ANI_MARIO_CARRY_IDLE_RIGHT;
			else return ID_ANI_MARIO_CARRY_IDLE_LEFT;
		}
		// bracing
		if (stateX == MARIO_STATE_X_BRACING) {
			if (nx > 0) return ID_ANI_MARIO_CARRY_WALKING_RIGHT;
			else return ID_ANI_MARIO_CARRY_WALKING_LEFT;
		}
		// can fly
		if (stateX == MARIO_STATE_X_RUNNING && IsCanFly()) {
			if (nx > 0) return ID_ANI_MARIO_CARRY_RUNNING_RIGHT;
			else return ID_ANI_MARIO_CARRY_RUNNING_LEFT;
		}
		// walking and running
		if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING || stateX == MARIO_STATE_X_RUNNING) {
			if (nx > 0) return ID_ANI_MARIO_CARRY_WALKING_RIGHT;
			else return ID_ANI_MARIO_CARRY_WALKING_LEFT;
		}

	}

	// STATE
	if (state == MARIO_STATE_SITTING) {
		if (nx > 0) return ID_ANI_MARIO_SIT_RIGHT;
		else return ID_ANI_MARIO_SIT_LEFT;
	}

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
		if (nx > 0) return ID_ANI_MARIO_FALL_RIGHT;
		else return ID_ANI_MARIO_FALL_LEFT;
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
	if (state == MARIO_STATE_IN_TUNEL) {
		return ID_ANI_MARIO_CAT_TUNNEL;
	}

	if (state == MARIO_STATE_CARRY) {
		// IN AIR
		// jumping
		if (stateY == MARIO_STATE_Y_JUMPING) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_JUMP_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_JUMP_LEFT;
		}
		// falling
		if (stateY == MARIO_STATE_Y_FALLING) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_FALL_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_FALL_LEFT;
		}
		// slow falling
		if (stateY == MARIO_STATE_Y_SLOWFALLING) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_SLOWFALL_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_SLOWFALL_LEFT;
		}
		// flying
		if (stateY == MARIO_STATE_Y_FLYING) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_FLYING_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_FLYING_LEFT;
		}

		// GROUNDING
		// idle
		if (stateX == MARIO_STATE_X_IDLE) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_IDLE_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_IDLE_LEFT;
		}
		// bracing
		if (stateX == MARIO_STATE_X_BRACING) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_WALKING_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_WALKING_LEFT;
		}
		// can fly
		if (stateX == MARIO_STATE_X_RUNNING && IsCanFly()) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_RUNNING_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_RUNNING_LEFT;
		}
		// walking and running
		if (stateX == MARIO_STATE_X_WALKING || stateX == MARIO_STATE_X_WALK_STOPPING || stateX == MARIO_STATE_X_RUNNING) {
			if (nx > 0) return ID_ANI_MARIO_CAT_CARRY_WALKING_RIGHT;
			else return ID_ANI_MARIO_CAT_CARRY_WALKING_LEFT;
		}
	}


	// STATE
	//sitting
	if (state == MARIO_STATE_SITTING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_SIT_RIGHT;
		else return ID_ANI_MARIO_CAT_SIT_LEFT;
	}
	//attacking
	if (state == MARIO_STATE_ATTACK) {
		if (nx > 0) return ID_ANI_MARIO_ATTACK_RIGHT;
		else return ID_ANI_MARIO_ATTACK_LEFT;
	}


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
		if (nx > 0) return ID_ANI_MARIO_CAT_FALL_RIGHT;
		else return ID_ANI_MARIO_CAT_FALL_LEFT;
	}
	// slow falling
	if (stateY == MARIO_STATE_Y_SLOWFALLING) {
		if (nx > 0) return ID_ANI_MARIO_CAT_SLOWFALL_RIGHT;
		else return ID_ANI_MARIO_CAT_SLOWFALL_LEFT;
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

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level==MARIO_LEVEL_CAT)
	{
		if (state == MARIO_STATE_SITTING)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2 + 1;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT + 1;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2 + 1;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT + 1;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2 + 1;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2 + 1;
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
	if (stateY == MARIO_STATE_Y_FLYING || stateY == MARIO_STATE_Y_SLOWFALLING) {
		SetStateY(MARIO_STATE_Y_FALLING);
	}
	if (level == MARIO_LEVEL_SMALL)
	{
		CGame* g = CGame::GetInstance();
		SetState(MARIO_STATE_DIE);
	}
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

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	if (this->state == MARIO_STATE_SITTING && state != MARIO_STATE_SITTING)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_BIG_SITTING_BBOX_HEIGHT) / 2;
	}
	if (state == MARIO_STATE_ATTACK) {
		attackStart = GetTickCount64();
		LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT>& objects = s->GetObjects();
		objects.push_back(new CAttackBlock(x + nx * (MARIO_BIG_BBOX_WIDTH), y, nx));
	}
	if (state == MARIO_STATE_DIE) {
		CGame* g = CGame::GetInstance();
		g->IncreaseHeart(-1);
		vy = -MARIO_JUMP_SPEED_Y;
		vx = 0.0f;
		ax = 0.0f;
		dieStart = GetTickCount64();
		CGame::GetInstance()->SetLevel(MARIO_LEVEL_SMALL);
	}
	if (state == MARIO_STATE_WINNING) {
		winStart = GetTickCount64();
		CGame::GetInstance()->SetLevel(level);
	}
	CGameObject::SetState(state);
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

void CMario::SaveToFile() {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	LPCWSTR saveFile = s->GetSaveFile();

	LPGAMEOBJECT marioMap = new CMarioMap(0, 0);
	LPGAMEOBJECT prevMarioMap = new CMarioMap(0, 0);
	vector<CGateConnection*> gateConnections;
	vector<CGate*> gates;

	ifstream inputf;
	inputf.open(saveFile);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (inputf.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[GATE_CONNECTIONS]") { section = SCENE_SECTION_GATE_CONNECTIONS; continue; }
		if (line == "[GATES]") { section = SCENE_SECTION_GATES; continue; }
		if (line == "[PLAYER]") { section = SCENE_SECTION_PLAYER; continue; }
		if (line == "[PREV_PLAYER]") { section = SCENE_SECTION_PREV_PLAYER; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_GATE_CONNECTIONS: {
			vector<string> tokens = split(line);

			if (tokens.size() < 4 || tokens[0] == "") break;
			int blockingLeft = 0;
			int blockingTop = 0;
			int blockingRight = 0;
			int blockingBottom = 0;
			int xCell = atof(tokens[0].c_str());
			int yCell = atof(tokens[1].c_str());
			int isBlocking = atof(tokens[2].c_str());
			int isNode = atof(tokens[3].c_str());

			if (tokens.size() == 8) {
				blockingLeft = atof(tokens[4].c_str());
				blockingTop = atof(tokens[5].c_str());
				blockingRight = atof(tokens[6].c_str());
				blockingBottom = atof(tokens[7].c_str());
			}
			gateConnections.push_back(new CGateConnection(xCell, yCell, isBlocking, isNode, blockingLeft, blockingTop, blockingRight, blockingBottom));
			break;
		}
		case SCENE_SECTION_GATES: {
			vector<string> tokens = split(line);

			if (tokens.size() < 4 || tokens[0] == "") break;
			int xCell = atof(tokens[0].c_str());
			int yCell = atof(tokens[1].c_str());
			int isCompleted = atof(tokens[2].c_str());
			int sceneId = atof(tokens[3].c_str());
			gates.push_back(new CGate(xCell, yCell, isCompleted, sceneId));
			break;
		}
		case SCENE_SECTION_PLAYER: {
			vector<string> tokens = split(line);

			if (tokens.size() < 2 || tokens[0] == "") break;
			int xCell = atof(tokens[0].c_str());
			int yCell = atof(tokens[1].c_str());
			delete marioMap;
			marioMap = new CMarioMap(xCell, yCell);
			break;
		}
		case SCENE_SECTION_PREV_PLAYER: {
			vector<string> tokens = split(line);

			if (tokens.size() < 2 || tokens[0] == "") break;
			int xCell = atof(tokens[0].c_str());
			int yCell = atof(tokens[1].c_str());
			delete prevMarioMap;
			prevMarioMap = new CMarioMap(xCell, yCell);
			break;
		}
		}
	}

	inputf.close();

	// Complete gate
	if (state != MARIO_STATE_DIE) {
		for (int i = 0; i < gates.size(); i++) {
			if (gates[i]->GetSceneId() == s->GetId()) {
				gates[i]->SetCompleted(true);
				break;
			}
		}
	}



	// Create and open a text file
	ofstream f(saveFile);

	// Write gate connection
	f << "[GATE_CONNECTIONS]" << endl;
	for (int i = 0; i < gateConnections.size(); i++) {
		CGateConnection* gc = gateConnections[i];
		f << gc->GetXCell() << "\t" << gc->GetYCell() << "\t" << gc->IsBlocking() << "\t" << gc->IsNode() << "\t" <<
			gc->IsBlockingLeft() << "\t" << gc->IsBlockingTop() << "\t" << gc->IsBlockingRight() << "\t" << gc->IsBlockingBottom() << endl;
	}

	// Write gate
	f << "[GATES]" << endl;
	for (int i = 0; i < gates.size(); i++) {
		CGate* gate = gates[i];
		f << gate->GetXCell() << "\t" << gate->GetYCell() << "\t" << gate->IsCompleted() << "\t" << gate->GetSceneId() << endl;
	}

	// Write marioMap
	f << "[PLAYER]" << endl;
	f << ((CMarioMap*)marioMap)->GetXCell() << "\t" << ((CMarioMap*)marioMap)->GetYCell() << endl;

	// Write prev player
	f << "[PREV_PLAYER]" << endl;
	if (state == MARIO_STATE_DIE) {
		f << ((CMarioMap*)prevMarioMap)->GetXCell() << "\t" << ((CMarioMap*)prevMarioMap)->GetYCell() << endl;
	}
	else {
		f << ((CMarioMap*)marioMap)->GetXCell() << "\t" << ((CMarioMap*)marioMap)->GetYCell() << endl;
	}

	// Close the file
	f.close();
}