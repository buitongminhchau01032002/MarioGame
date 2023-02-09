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
	case DIK_S: {
		CGame* g = CGame::GetInstance();
		CWorldMapScene* scene = dynamic_cast<CWorldMapScene*>(g->GetCurrentScene());
		vector<CGate*> gates = scene->GetGate();
		CGate* gate = NULL;
		for (int i = 0; i < gates.size(); i++) {
			if (gates[i]->GetXCell() == mario->GetXCell() && gates[i]->GetYCell() == mario->GetYCell()) {
				gate = gates[i];
				break;
			}
		}
		if (gate) {
			g->InitiateSwitchScene(gate->GetSceneId());
		}
		break;
	}
	case DIK_DOWN: {
		mario->GoDown();
		break;
	}
	case DIK_UP: {
		mario->GoUp();
		break;
	}
	case DIK_LEFT: {
		mario->GoLeft();
		break;
	}
	case DIK_RIGHT: {
		mario->GoRight();
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