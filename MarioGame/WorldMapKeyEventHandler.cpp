#include "WorldMapKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "WorldMapScene.h"

void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMarioMap* mario = (CMarioMap*)((CWorldMapScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	
	case DIK_DOWN: {
		mario->GoToCell(mario->GetXCell(), mario->GetYCell() + 1);
		break;
	}
	case DIK_UP: {
		mario->GoToCell(mario->GetXCell(), mario->GetYCell() - 1);
		break;
	}
	case DIK_LEFT: {
		mario->GoToCell(mario->GetXCell() - 1, mario->GetYCell());
		break;
	}
	case DIK_RIGHT: {
		mario->GoToCell(mario->GetXCell() + 1, mario->GetYCell());
		break;
	}

	case DIK_A:
		
		break;
	case DIK_1:
		
		break;
	case DIK_2:
		break;
	case DIK_3:
		break;
	case DIK_0:
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CWorldMapKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	//CMarioMap* mario = (CMarioMap*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		break;
	case DIK_DOWN:
		break;
	}
}

void CWorldMapKeyHandler::KeyState(BYTE* states)
{
	
}