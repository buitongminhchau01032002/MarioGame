#include "WorldMapKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "PlayScene.h"

void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	switch (KeyCode)
	{
	case DIK_S: {

		
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

	//CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
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