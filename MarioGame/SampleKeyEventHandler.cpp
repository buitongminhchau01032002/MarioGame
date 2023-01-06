#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->GetLevel() != MARIO_LEVEL_CAT) {
			mario->SetState(MARIO_STATE_JUMP);
		}
		else {
			// Handle press "S" when is Cat

			if (mario->GetIsFlying()) {
				// boost to fly
				mario->SetState(MARIO_STATE_FLY);
			}
			else if (mario->GetCanFly()) {
				// Start fly
				mario->SetState(MARIO_STATE_FLY);
			}
			else {
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}

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
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S: {
		if (mario->GetLevel() != MARIO_LEVEL_CAT) {
			mario->SetState(MARIO_STATE_RELEASE_JUMP);
		}
		else { // Cat
			if (!mario->GetIsFlying()) {
				mario->SetState(MARIO_STATE_RELEASE_JUMP);
			}
		}
		break;
	}
		
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->Setnx(1);
		
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING);
		else
			mario->SetState(MARIO_STATE_WALKING);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->Setnx(-1);
		
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING);
		else
			mario->SetState(MARIO_STATE_WALKING);
	}
	else {
		mario->SetState(MARIO_STATE_IDLE);
	}
}