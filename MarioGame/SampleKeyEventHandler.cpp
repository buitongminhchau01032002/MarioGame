#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario *)(scene)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) {
		return;
	}

	switch (KeyCode)
	{
	case DIK_S: {
		
		if (mario->GetLevel() == MARIO_LEVEL_CAT && mario->GetStateY() == MARIO_STATE_Y_FLYING) {
			mario->BoostFly();
		} else if (mario->GetLevel() == MARIO_LEVEL_CAT && !mario->IsCanFly()) {
			if (mario->GetStateY() == MARIO_STATE_Y_FALLING || mario->GetStateY() == MARIO_STATE_Y_SLOWFALLING)
				mario->SetStateY(MARIO_STATE_Y_SLOWFALLING);
			else
				mario->SetStateY(MARIO_STATE_Y_JUMPING);
		} else if (mario->GetLevel() == MARIO_LEVEL_CAT && mario->IsCanFly()) {
			mario->SetStateY(MARIO_STATE_Y_FLYING);
		} else if (mario->GetStateY() == MARIO_STATE_Y_GROUND) {
			mario->SetStateY(MARIO_STATE_Y_JUMPING);
		}
		break;
	}

	case DIK_A:
		if (mario->GetLevel() == MARIO_LEVEL_CAT && mario->GetState() != MARIO_STATE_ATTACK && mario->GetState() != MARIO_STATE_CARRY) {
			mario->SetState(MARIO_STATE_ATTACK);
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_CAT);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;

	// TEST CAMERA
	case DIK_8:
		scene->SwitchCamera(100);
		break;
	case DIK_9:
		scene->SwitchCamera(200);
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) {
		return;
	}
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetStateY() == MARIO_STATE_Y_JUMPING) {
			mario->BreakJump();
		}
		break;
	case DIK_DOWN:
		if (mario->GetState() == MARIO_STATE_SITTING) {
			mario->SetState(MARIO_STATE_NONE);
		}
		break;
	case DIK_A:
		if (mario->GetState() == MARIO_STATE_CARRY) {
			mario->SetState(MARIO_STATE_NONE);
		}
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) {
		return;
	}
	float vx, vy;
	mario->GetSpeed(vx, vy);

	mario->SetAKeyPress(false);
	if (game->IsKeyDown(DIK_A)) mario->SetAKeyPress(true);

	mario->SetDownKeyPress(false);
	if (game->IsKeyDown(DIK_DOWN)) mario->SetDownKeyPress(true);

	mario->SetUpKeyPress(false);
	if (game->IsKeyDown(DIK_UP)) mario->SetUpKeyPress(true);

	if (game->IsKeyDown(DIK_DOWN) && mario->GetState() != MARIO_STATE_IN_TUNEL && mario->GetLevel() != MARIO_LEVEL_SMALL && mario->GetStateY() == MARIO_STATE_Y_GROUND && mario->GetState() != MARIO_STATE_CARRY) {
		mario->SetState(MARIO_STATE_SITTING);
		if (vx == 0) {
			mario->SetStateX(MARIO_STATE_X_IDLE);
		}
		else {
			mario->SetStateX(MARIO_STATE_X_WALK_STOPPING);
		}
		return;
	}
	if (game->IsKeyDown(DIK_RIGHT))	{
		mario->Setnx(1);
		if (
			((mario->GetStateX() == MARIO_STATE_X_WALKING || mario->GetStateX() == MARIO_STATE_X_RUNNING || mario->GetStateX() == MARIO_STATE_X_WALK_STOPPING) && vx < 0) ||
			(mario->GetStateX() == MARIO_STATE_X_BRACING && vx < 0)
		) {
			mario->SetStateX(MARIO_STATE_X_BRACING);
		}
		else {
			mario->SetStateX(MARIO_STATE_X_WALKING);
			if (game->IsKeyDown(DIK_A)) {
				mario->SetStateX(MARIO_STATE_X_RUNNING);
			}
		}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->Setnx(-1);
		if (
			((mario->GetStateX() == MARIO_STATE_X_WALKING || mario->GetStateX() == MARIO_STATE_X_RUNNING || mario->GetStateX() == MARIO_STATE_X_WALK_STOPPING) && vx > 0) ||
			(mario->GetStateX() == MARIO_STATE_X_BRACING && vx > 0)
			) {
			mario->SetStateX(MARIO_STATE_X_BRACING);
		}
		else {
			mario->SetStateX(MARIO_STATE_X_WALKING);
			if (game->IsKeyDown(DIK_A)) {
				mario->SetStateX(MARIO_STATE_X_RUNNING);
			}
		}
	}
	else if (mario->GetStateX() != MARIO_STATE_X_IDLE)
		mario->SetStateX(MARIO_STATE_X_WALK_STOPPING);
}