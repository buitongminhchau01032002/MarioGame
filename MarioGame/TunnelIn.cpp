#include "TunnelIn.h"
#include "PlayScene.h"
#include "Mario.h"
#include "TunnelOut.h"

void CTunnelIn::Render()
{
	CSprites::GetInstance()->Get(spriteId)->Draw(x+1, y);
	RenderBoundingBox();
}

void CTunnelIn::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)(scene)->GetPlayer();

	// Check can goto
	if (mario->GetState() != MARIO_STATE_IN_TUNEL && IsCanGoTo()) {
		mario->GoToTunnel(ny, id);
	}
	else if (mario->GetTunnelId() == id) {
		// Check in tunnel
		float marioX, marioY;
		mario->GetPosition(marioX, marioY);

		// Check go to tunnel out
		if ((ny > 0 && marioY > y) || (ny < 0 && marioY < y)) {

			CTunnelOut* tunnelOut = NULL;
			vector<LPGAMEOBJECT>& objects = scene->GetObjects();
			for (int i = 0; i < objects.size(); i++) {
				tunnelOut = dynamic_cast<CTunnelOut*>(objects[i]);
				if (tunnelOut && tunnelOut->GetId() == tunnelOutId) {
					break;
				}
				tunnelOut = NULL;
			}
			
			if (!tunnelOut) {
				mario->SetState(MARIO_STATE_NONE);
				return;
			}

			// Move mario to tunnel out
			float tunnelOutX, tunnelOutY;
			tunnelOut->GetPosition(tunnelOutX, tunnelOutY);
			mario->SetPosition(tunnelOutX, tunnelOutY);
			// Set goto tunnel for mario
			mario->GoToTunnel(tunnelOut->GetNy(), tunnelOutId);
			// set camera
			scene->SwitchCamera(tunnelOut->GetCameraId());
		}
	}
}

bool CTunnelIn::IsCanGoTo() {
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)(scene)->GetPlayer();
	float mario_l, mario_t, mario_r, mario_b;
	mario->GetBoundingBox(mario_l, mario_t, mario_r, mario_b);

	float tunnel_l, tunnel_t, tunnel_r, tunnel_b;
	this->GetBoundingBox(tunnel_l, tunnel_t, tunnel_r, tunnel_b);
	if (tunnel_l < mario_l && tunnel_r > mario_r && (
		(ny > 0 && mario_b < tunnel_b && mario_b > tunnel_t) ||
		(ny < 0 && mario_t > tunnel_t && mario_t < tunnel_b))) {
		return true;
	}
	return false;
}