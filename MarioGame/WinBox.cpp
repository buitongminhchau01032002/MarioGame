#include "WinBox.h"
#include "PlayScene.h"
#include "Animation.h"

void CWinBox::Render() {
	int aniId = ID_ANI_WIN_BOX - item;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CWinBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;

	if (isUnboxed) {
		y -= WIN_BOX_SPEED;
		if (GetTickCount64() - removeStart > WIN_BOX_DURATION)
		{
			this->Delete();
		}
	}
	else {
		if (GetTickCount64() - changItemStart > WIN_BOX_STEP_TIME)
		{
			changItemStart = GetTickCount64();
			item++;
			if (item > 3) {
				item = 1;
			}
		}
	}

}

int CWinBox::Unbox() {
	isUnboxed = true;
	removeStart = GetTickCount64();
	return item;
}