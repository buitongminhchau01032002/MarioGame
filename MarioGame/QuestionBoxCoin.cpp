#include "QuestionBoxCoin.h"
#include "PlayScene.h"
#include "CoinFlyUp.h"

void CQuestionBoxCoin::Unbox()
{
	if (state == QUESTION_BOX_STATE_ORIGIN) {
		unbox_start = GetTickCount64();
		state = QUESTION_BOX_STATE_UNBOXING;
		LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT>& objects = s->GetObjects();
		objects.push_back(new CCoinFlyUp(x, y));
	}
}
