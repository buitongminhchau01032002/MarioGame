#include "StartSceneKeyHandler.h"

#include "debug.h"
#include "Game.h"

#include "StartScene.h"

void CStartKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (KeyCode)
	{
	case DIK_W:
		CGame::GetInstance()->InitiateSwitchScene(0);
		break;
	
	}
}

void CStartKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_S:
		break;
	case DIK_DOWN:
		break;
	}
}

void CStartKeyHandler::KeyState(BYTE* states)
{

}
