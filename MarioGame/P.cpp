#include "P.h"
#include "debug.h"
#include "PlayScene.h"
#include "LightBrick.h"

void CP::Render()
{
	if (state == P_STATE_DELAYING) return;
	CAnimations* animations = CAnimations::GetInstance();
	if (state != P_STATE_DOWN)
		animations->Get(ID_ANI_P_SHOW)->Render(x, y);
	else
		animations->Get(ID_ANI_P_DOWN)->Render(x, y);
	//RenderBoundingBox();
}

void CP::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;

	if (GetTickCount64() - startDelay < P_STARTUP_DELAY) return;
	if (state == P_STATE_DELAYING) state = P_STATE_STARTUP;
	if (state == P_STATE_STARTUP) {
		y -= dt * P_STARTUP_SPEED;
		if (originY - y > P_HEIGHT) {
			y = originY - P_HEIGHT;
			state = P_STATE_SHOW;
		}
	}
}

void CP::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - P_WIDTH / 2;
	t = y - P_HEIGHT / 2 + 5;
	r = l + P_WIDTH;
	b = t + P_HEIGHT - 5;
}

void CP::Press() {
	state = P_STATE_DOWN;
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT>& objects = s->GetObjects();

	for (int i = 0; i < objects.size(); i++) {
		CLightBrick* brick = dynamic_cast<CLightBrick*>(objects[i]);
		if (brick && brick->GetGroup() == group) {
			brick->ToCoin();
		}
	}
}