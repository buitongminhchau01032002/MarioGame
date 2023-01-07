#include "Chomper.h"
#include "PlayScene.h"
#include "ChomperBullet.h"


CChomper::CChomper(float x, float y) :CGameObject(x, y)
{
	originY = y;
	state = CHOMPER_STATE_HIDDEN;
	pushTimerStart = GetTickCount64();
	shootingTimerStart = 0;
}
float CChomper::GetGunY()
{
	return y + CHOMPER_GUN_X_OFFSET;
}

void CChomper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
	left = x - CHOMPER_BBOX_WIDTH / 2;
	top = y - CHOMPER_BBOX_HEIGHT / 2;
	right = left + CHOMPER_BBOX_WIDTH;
	bottom = top + CHOMPER_BBOX_HEIGHT;
	
}

void CChomper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	if (!s->IsInScreenBounding(x, y)) return;
	

	if (state == CHOMPER_STATE_HIDDEN) {
		y += CHOMPER_SHOW_SPEED * dt;
		if (y > originY) {
			y = originY;
		}
		if (GetTickCount64() - pushTimerStart > CHOMPER_HIDDEN_TIME) {
			state = CHOMPER_STATE_SHOW;
		}
	}
	else if (state == CHOMPER_STATE_SHOW) {
		y -= CHOMPER_SHOW_SPEED * dt;
		if (y < originY - CHOMPER_PUSH_HEIGHT) {
			y = originY - CHOMPER_PUSH_HEIGHT;
			state = CHOMPER_STATE_SHOOTING;
			shootingTimerStart = GetTickCount64();
		}
	}
	else {
		if (GetTickCount64() - shootingTimerStart > CHOMPER_SHOOTING_DURATION) {
			LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
			LPGAMEOBJECT player = s->GetPlayer();

			float playerX, playerY;
			player->GetPosition(playerX, playerY);
			
			if (abs(playerX - x) < CHOMPER_SHORT_DISTANCE) {

				vector<LPGAMEOBJECT>& objects = s->GetObjects();
				objects.push_back(new CChomperBullet(x, GetGunY(), playerX - x, playerY - GetGunY()));
			}


			state = CHOMPER_STATE_HIDDEN;
			pushTimerStart = GetTickCount64();
		}
		
	}
	
}

void CChomper::Render()
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	LPGAMEOBJECT player = s->GetPlayer();

	float playerX, playerY;
	player->GetPosition(playerX, playerY);
	int aniId;
	if (playerY < GetGunY()) {
		if (playerX < x) {
			aniId = ID_ANI_CHOMPER_LEFT_TOP;
			if (state == CHOMPER_STATE_SHOOTING) {
				aniId = ID_ANI_CHOMPER_LEFT_TOP_SHOOTING;
			}
		}
		else {
			aniId = ID_ANI_CHOMPER_RIGHT_TOP;
			if (state == CHOMPER_STATE_SHOOTING) {
				aniId = ID_ANI_CHOMPER_RIGHT_TOP_SHOOTING;
			}
		}
	} else {
		if (playerX < x) {
			aniId = ID_ANI_CHOMPER_LEFT_BOTTOM;
			if (state == CHOMPER_STATE_SHOOTING) {
				aniId = ID_ANI_CHOMPER_LEFT_BOTTOM_SHOOTING;
			}
		}
		else {
			aniId = ID_ANI_CHOMPER_RIGHT_BOTTOM;
			if (state == CHOMPER_STATE_SHOOTING) {
				aniId = ID_ANI_CHOMPER_RIGHT_BOTTOM_SHOOTING;
			}
		}
	}


	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	CAnimations::GetInstance()->Get(ID_ANI_CHOMPER_PIPE)->Render(x-1, originY-8);
	//RenderBoundingBox();
}