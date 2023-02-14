#include "QuestionBoxP.h"
#include "PlayScene.h"
#include "P.h"
#include "Leaf.h"
#include "Mario.h"
#include "LightBrick.h"

void CQuestionBoxP::Unbox()
{
	if (state == QUESTION_BOX_STATE_ORIGIN) {
		unbox_start = GetTickCount64();
		state = QUESTION_BOX_STATE_UNBOXING;

		LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT>& objects = s->GetObjects();
		// Find and insert P before box
		int index = -1;
		for (int i = 0; i < objects.size(); i++) {
			if ((CQuestionBoxP*)(objects[i]) == this) {
				index = i;
				break;
			}
		}

		if (index != -1) {
			objects.insert(objects.begin() + index, new CP(x, y, group));
		}

		//objects.push_back(new CCoinFlyUp(x, y));
	}
}

void CQuestionBoxP::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	if (state == QUESTION_BOX_STATE_ORIGIN)
		animations->Get(ID_ANI_LIGHT_BRICK)->Render(x, y - 1);
	else
		animations->Get(ID_ANI_QUESTION_BOX + 1)->Render(x, y - 1);
}