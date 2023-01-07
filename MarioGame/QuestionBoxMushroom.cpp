#include "QuestionBoxMushroom.h"
#include "PlayScene.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Mario.h"

void CQuestionBoxMushroom::Unbox()
{
	if (state == QUESTION_BOX_STATE_ORIGIN) {
		unbox_start = GetTickCount64();
		state = QUESTION_BOX_STATE_UNBOXING;

		LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT>& objects = s->GetObjects();

		if (((CMario*)s->GetPlayer())->GetLevel() >= MARIO_LEVEL_BIG) {
			objects.push_back(new CLeaf(x, y));
			return;
		}

		// Find and insert mushroom before box
		int index = -1;
		for (int i = 0; i < objects.size(); i++) {
			if ((CQuestionBoxMushroom*)(objects[i]) == this) {
				index = i;
				break;
			}
		}

		if (index != -1) {
			objects.insert(objects.begin() + index, new CMushroom(x, y));
		}

		//objects.push_back(new CCoinFlyUp(x, y));
	}
}
